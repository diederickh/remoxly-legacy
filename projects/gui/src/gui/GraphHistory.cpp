#include <gui/GraphHistory.h>
#include <gui/Render.h>
#include <gui/Group.h>
#include <gui/Utils.h>

namespace rx { 

GraphHistory::GraphHistory(std::string label, int size, float minv, float maxv, float height )
  :Widget(GUI_TYPE_NONE, label)
  ,value(value)
  ,text_x(0)
  , frameRate( 0 )
  , frameCounter( 0 )
  , fpsTimeAccum( 0.0f )
  , currMinValue( minv )
  , currMaxValue( minv )
{
	oneOverMax = 1.0f / (float)maxv;
	oneOverValuesSize = 1.0f / (float)size;
    tailIndex = 0;
    headIndex = 1;
	enableFPS = true;
    fpsTimeAccum = 0.0f;
    frameCounter = 0;
    frameRate = 0;

	this->minv = minv;
	this->maxv = maxv;

	headValue = minv;

	values.resize( size + 1 );
    for( int i=0; i<values.size(); i++ )
	{
        values[i] = minv;
	}

	verts.resize( values.size() * 2 );

	h = height;
}

void GraphHistory::create() {

  render->addRectangle(x, y, w - group->padding, h, group->bg_color, true, 0.0, 0.2f);
  //render->writeText(x + group->xindent, y + group->yindent, label, group->label_color);

  std::string fpsStr = std::to_string( currMaxValue ) + " : " + std::to_string( frameRate );
  render->writeText( x + group->xindent, y + group->yindent, fpsStr, group->label_color );
  //render->writeText( x + (w-fpsStr.size()*8.5f) + group->xindent, y + group->yindent, fpsStr, group->label_color );

  render->addLineStrip( (int)values.size(), &verts[0], group->line_color );
}

/*
void GraphHistory::onCharPress(unsigned int key) {

  if(state & GUI_STATE_EDITABLE) {
    render->onCharPress(key);
  }
}

void GraphHistory::onKeyPress(int key, int modkeys) {
  if(state & GUI_STATE_EDITABLE) {
    if(key == GUI_KEY_ENTER) {
      disableInputAndCopyValue();
    }
    else {
      render->onKeyPress(key, modkeys);
    }
  }
}

void GraphHistory::onMousePress(float mx, float my, int button, int modkeys) {

  // text input
  if(GUI_IS_INSIDE(mx, my, text_x, y, text_w, h)) {
    if(!(state & GUI_STATE_EDITABLE)) {
      state |= GUI_STATE_EDITABLE;
      needs_redraw = true;
    }
  }
  else {
    if(state & GUI_STATE_EDITABLE) {
      disableInputAndCopyValue();
    }
  }
}

void GraphHistory::onMousePressOutside(float mx, float my, int button, int modkeys) {

  if(state & GUI_STATE_EDITABLE) {
    disableInputAndCopyValue();
  }
}

void GraphHistory::disableInputAndCopyValue() {
  
  render->getTextInputValue(value);

  render->disableTextInput();

  state &= ~GUI_STATE_EDITABLE;
  needs_redraw = true;

  notify(GUI_EVENT_VALUE_CHANGED);
}
*/

void GraphHistory::addValue(float t)
{
    headValue = gui_clamp<float>( t, minv, maxv );

	// Compute FPS
	if (enableFPS)
	{
		frameCounter++;
		fpsTimeAccum += headValue;
		if (fpsTimeAccum >= 1000.0f)
		{
			frameRate = frameCounter;
			fpsTimeAccum = 0.0f;
			frameCounter = 0;
		}
	}

	if( ! values.empty() )
	{
		values[ headIndex ] = headValue;
		// Find min and max value so far
		if( headValue < currMinValue )
			currMinValue = headValue;
		if( headValue > currMaxValue )
			currMaxValue = headValue;
		currMaxValue *= 1.0f - 0.005f; // Damping
		currMaxValue = gui_clamp<float>( currMaxValue, currMinValue, currMaxValue );

		// Ring-buffer style
		tailIndex = headIndex;
		headIndex++;
		headIndex = headIndex >= values.size() ? 0 : headIndex;
		tailIndex = tailIndex >= values.size() ? 0 : tailIndex;
	}

	int count = 0;
    while( count < values.size() )
    {
        int idx = headIndex + count;
        idx = idx % values.size();
                    
        float per = count * oneOverValuesSize;
        float xx = x + w * per; // move right to left
        float normValue = (values[ idx ]-minv) * oneOverMax;
		float yy = y + (h - normValue*h);
		verts[ count * 2 + 0 ] = xx;
		verts[ count * 2 + 1 ] = yy;
        count ++;
    }        

	needs_redraw = true;
}

} // namespace rx
