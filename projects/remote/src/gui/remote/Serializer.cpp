#include <sstream>
#include <gui/remote/Utils.h>
#include <gui/remote/Serializer.h>
#include <gui/remote/Types.h>

bool Serializer::serialize(std::string& result) {
  
  if(!groups.size() && !panels.size()) {
    printf("Error: cannot serialize because no groups or panels have been added.\n");
    return false;
  }

  json_t* js_all = json_object();

  if(!js_all) {
    printf("Error: cannot allocate the main container for the serialized groups and panels.\n");
    return false;
  }

  json_t* js_panels = NULL;
  json_t* js_groups = NULL;

  // serialize panels
  if(panels.size()) {
    
    js_panels = serializePanels(panels);

    if(!js_panels) {
      REMOXLY_FREE_JSON(js_all);
      return false;
    }

    if(json_object_set(js_all, "ps", js_panels) != 0) {
      printf("Error: cannot set the serialized panels.\n");
      REMOXLY_FREE_JSON(js_panels);
      REMOXLY_FREE_JSON(js_all);
      return false;
    }
  }

  // serialize separate groups
  if(groups.size()) {

    js_groups = serializeGroups(groups);

    if(!js_groups) {
      REMOXLY_FREE_JSON(js_panels);
      REMOXLY_FREE_JSON(js_all);
      return false;
    }

    if(json_object_set(js_all, "g", js_groups) != 0) {
      printf("Error: cannot set the groups in the main serialized container.\n");
      REMOXLY_FREE_JSON(js_panels);
      REMOXLY_FREE_JSON(js_groups);
      REMOXLY_FREE_JSON(js_all);
      return false;
    }
  }
  
  result = json_dumps(js_all, JSON_COMPACT);

  REMOXLY_FREE_JSON(js_panels);
  REMOXLY_FREE_JSON(js_groups);
  REMOXLY_FREE_JSON(js_all);

  return true;
}

// creates an array of panels
json_t* Serializer::serializePanels(std::vector<Panel*>& all) {

  if(!all.size()) {
    printf("Warning: trying to serialize panels, but given vector is empty.\n");
    return NULL;
  }

  json_t* js_panels = json_array();

  if(!js_panels) {
    printf("Error: cannot allocate a js_panels array.\n");
    return NULL;
  }

  for(std::vector<Panel*>::iterator it = all.begin(); it != all.end(); ++it) {

    json_t* js_panel = serializePanel((*it));

    if(!js_panel) {
      printf("Warning: serializing a panel failed.\n");
      continue;
    }

    if(json_array_append(js_panels, js_panel) != 0) {
      printf("Error: cannot append panel to array of panels.\n");
      REMOXLY_FREE_JSON(js_panel);
    }
  }

  return js_panels;
}

// serialize the group; sets it to parent
json_t* Serializer::serializeGroup(Group* group) {

  if(!group) {
    printf("Error: cannot serialize the group. Invalid group pointer given.\n");
    return NULL;
  }

  json_t* js_group = json_object();

  if(!js_group) {
    printf("Error: cannot allocate the group js object.\n");
    return NULL;
  }

  json_t* js_widgets = json_array();

  if(!js_widgets) {
    printf("Error: cannot allocate a new json_array() for widgets.\n");
    return NULL;
  }

  for(std::vector<Widget*>::iterator it = group->children.begin(); it != group->children.end(); ++it) {

    Widget* wid = *it;

    switch(wid->type) {

      case GUI_TYPE_SLIDER_FLOAT: {
        Slider<float>* slider = static_cast<Slider<float>* >(wid);
        json_t* js_slider = serializeSlider(slider);
        if(js_slider) {
          appendToArray(js_widgets, js_slider);
        }
        break;
      }

      case GUI_TYPE_SLIDER_INT: {
        Slider<int>* slider = static_cast<Slider<int>* >(wid);
        json_t* js_slider = serializeSlider(slider);
        if(js_slider) {
          appendToArray(js_widgets, js_slider);
        }
        break;
      }

      case GUI_TYPE_TOGGLE: {
        Toggle* toggle = static_cast<Toggle*>(wid);
        json_t* js_toggle = serializeToggle(toggle);
        if(js_toggle) {
          appendToArray(js_widgets, js_toggle);
        }
        break;
      }

      default: {
        printf("Warning: we're not capable of serializing the type: %d yet, for label: %s.\n", wid->type, wid->label.c_str());
        break;
      }
    }
  }
  
  if(json_object_set(js_group, "g", js_widgets) != 0) {
    // @todo - serializeGroup, do we need to free all elements in the array?
    printf("Error: cannot set the group widgets.\n");
    REMOXLY_FREE_JSON(js_widgets);
    return NULL;
  }

  return js_group;
}

// serializes the groups into the given parent array
json_t* Serializer::serializeGroups(std::vector<Group*>& all) {

  json_t* js_groups = json_array();

  if(!js_groups) {
    printf("Error: cannot allocate the groups container array.\n");
    return NULL;
  }

  if(!all.size()) {
    printf("Warning: not serializing groups vector. no elemens.\n");
    return NULL;
  }
  
  for(std::vector<Group*>::iterator it = all.begin(); it != all.end(); ++it) {

    json_t* js_group  =serializeGroup((*it));

    if(!js_group) {
      printf("Warning: serializing a group failed.\n");
      REMOXLY_FREE_JSON(js_group);
      continue;
    }

    if(json_array_append(js_groups, js_group) != 0) {
      // @todo - serializeGroups() - we probably want to clean up all allocated memory
      printf("Error: cannot append the groups to the array.\n");
      REMOXLY_FREE_JSON(js_group);
      continue;
    }
  }
  
  return js_groups;
}

json_t* Serializer::serializePanel(Panel* panel) {

  if(!panel) {
    printf("Error: cannot serialize the panel; invalid ptr.\n");
    return NULL;
  }

  json_t* js_panel = json_object();

  if(!js_panel) {
    printf("Error: cannot allocate a object for the panel.\n");
    return NULL;
  }
  
  json_t* js_groups = serializeGroups(panel->groups);

  if(!js_groups) {
    printf("Error: cannot serialize groups for the panel.\n");
    REMOXLY_FREE_JSON(js_panel);
    return NULL;
  }

  if(json_object_set(js_panel, "p", js_groups) != 0) {
    printf("Error: cannot set the `p` member of the panels container.\n");
    REMOXLY_FREE_JSON(js_groups);
    REMOXLY_FREE_JSON(js_panel);
    return NULL;
  }

  return js_panel;
}

bool Serializer::canSerialize() {
  return panels.size() || groups.size();
}

bool Serializer::appendToArray(json_t* parent, json_t* child) {

  if(!parent) {
    printf("Error: cannot append because given parent is not valid.\n");
    return false;
  }

  if(!child) {
    printf("Error: cannot append child because it's not valid.\n");
    return false;
  }

  int r = json_array_append(parent, child);
  json_decref(child);

  if(r != 0) {
    printf("Error: cannot append the child the the array; jansson gave error.\n");
    return false;
  }

  return true;
}

json_t* Serializer::serializeSlider(Slider<int>* slider) {

  json_t* js_slider = json_pack("{s:i,s:i,s:s,s:i,s:i,s:i,s:i}", 
                                "t", slider->type, 
                                "v", slider->value,
                                "l", slider->label.c_str(),
                                "i", slider->id,
                                "minv", slider->minv,
                                "maxv", slider->maxv,
                                "step", slider->step
                                );
  return js_slider;
}

json_t* Serializer::serializeSlider(Slider<float>* slider) {

  json_t* js_slider = json_pack("{s:i,s:f,s:s,s:i,s:f,s:f,s:f}", 
                                "t", slider->type, 
                                "v", slider->value,
                                "l", slider->label.c_str(),
                                "i", slider->id,
                                "minv", slider->minv,
                                "maxv", slider->maxv,
                                "step", slider->step
                                );
  return js_slider;
}

json_t* Serializer::serializeToggle(Toggle* toggle) {

  json_t* js_toggle = json_pack("{s:i,s:s,s:i}", 
                                "t", toggle->type,   
                                "l", toggle->label.c_str(),
                                "i", toggle->id);
  return js_toggle;
}


void Serializer::addGroup(Group* g) {
  groups.push_back(g);
}

void Serializer::addPanel(Panel* p) {
  panels.push_back(p);
}

void Serializer::clear() {
  panels.clear();
  groups.clear();
}

std::string Serializer::serializeTask(int task, std::string& value, int id) {

  std::stringstream ss;

  ss << "{" 
     <<  "\"t\":" << task << ",";

  if(value.size()) {
    ss <<  "\"v\":" << value << ",";
  }

   ss  <<  "\"i\":" << id
        << "}";

  std::string str = ss.str();

  return str;
}

bool Serializer::serializeChangedValue(Widget* w, std::string& json) {

  json_t* js_value = serializeValueWidget(w);

  if(!js_value) {
    return false;
  }

  json = json_dumps(js_value, JSON_COMPACT);

  REMOXLY_FREE_JSON(js_value);

  return true;
}

json_t* Serializer::serializeValueWidget(Widget* w) {

  switch(w->type) {

    case GUI_TYPE_SLIDER_INT: {
      Slider<int>* slider = static_cast<Slider<int>* >(w);
      json_t* js_value = serializeValueSliderInt(slider);
      return js_value;
    }

    case GUI_TYPE_SLIDER_FLOAT: { 
      Slider<float>* slider = static_cast<Slider<float> * >(w);
      json_t* js_value = serializeValueSliderFloat(slider);
      return js_value;
    }
      
    default: {
      printf("Warning: unhandled widget type for value changed: %s\n", w->label.c_str());
      return NULL;
    }
  }
  
  return NULL;
}

bool Serializer::serializeValues(std::string& json) {
  
  json_t* js_array = json_array();

  if(!js_array) {
    printf("Error: cannot create json array to serialize values.\n");
    return false;
  }

  for(std::vector<Panel*>::iterator pit = panels.begin(); pit != panels.end(); ++pit) {
    Panel* p = *pit;
    serializeValuesForPanel(p, js_array);
  }

  json = json_dumps(js_array, JSON_COMPACT);
  
  REMOXLY_FREE_JSON(js_array);
  return true;
}

bool Serializer::serializeValuesForPanel(Panel* p, json_t* array) {

  if(!p) {
    return false;
  }

  for(std::vector<Group*>::iterator git = p->groups.begin(); git != p->groups.end(); ++git) {
    serializeValuesForGroup((*git), array);
  }

  return true;
}

bool Serializer::serializeValuesForGroup(Group* g, json_t* array) { 

  if(!g) {
    return false;
  }

  for(std::vector<Widget*>::iterator it = g->children.begin(); it != g->children.end(); ++it) {

    Widget* w = *it;
    json_t* js_widget = serializeValueWidget(w);

    if(!js_widget) {
      printf("Error: cannot serialize the value for the widget: %s\n", w->label.c_str());
      continue;
    }

    appendToArray(array, js_widget);
  }
  return true;
}

json_t* Serializer::serializeValueSliderInt(Slider<int>* slider) {
  json_t* js_value = json_pack("{s:i,s:i}", "i", slider->id, "v", slider->value);
  return js_value;
}

json_t* Serializer::serializeValueSliderFloat(Slider<float>* slider) {
  json_t* js_value = json_pack("{s:i,s:f}", "i", slider->id, "v", slider->value);
  return js_value;
}

