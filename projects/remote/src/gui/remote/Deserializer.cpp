#include <stdio.h>
#include <gui/Remoxly.h>
#include <gui/remote/Generator.h>
#include <gui/remote/Deserializer.h>
#include <gui/remote/Utils.h>

Deserializer::Deserializer(Generator* gen)
  :gen(gen)
{
}

bool Deserializer::deserialize(const std::string& model) {

  if(!gen) {
    printf("Error: cannot deserialize, no generator set.\n");
    return false;
  }

  json_error_t err;
  json_t* root = json_loads(model.c_str(), 0, &err);

  if(!root) {
    printf("Error: cannot deserialize the given string; it contains an error: %s\n", err.text);
    json_decref(root);
    root = NULL;
    return false;
  }

  // find group elements
  json_t* js_groups = json_object_get(root, "g");
  if(json_is_array(js_groups)) {

    int num_groups = json_array_size(js_groups);

    for(int i = 0; i < num_groups; ++i) {

      json_t* js_group = json_array_get(js_groups, i);
      if(!json_is_array(js_group)) {
        printf("Error: the group definition seems to be invalid.\n");
        continue;
      }

      deserializeGroup(js_group);
    }
  }

  // find panels.
  json_t* js_panels = json_object_get(root, "ps");

  if(json_is_array(js_panels)) {

    int num_panels = json_array_size(js_panels);
    printf("> %d\n", num_panels);
    for(int i = 0; i < num_panels; ++i) {
      Panel* panel = deserializePanel(json_array_get(js_panels, i));
    }
  }

  json_decref(root);
  root = NULL;

  return true;
}

Panel* Deserializer::deserializePanel(json_t* el) {

  if(!el) {
    printf("Error: trying to deserialize a panel, but the given element is invalid.\n");
    return NULL;
  }

  if(!gen) {
    printf("Error: cannot deserialize the panel, no generator set.\n");
    return NULL;
  }

  json_t* js_groups = json_object_get(el, "p");

  if(!js_groups) {
    printf("Error: no `p` element found in the serialize panel.\n");
    return NULL;
  }

  Panel* p = gen->createPanel();

  if(!p) {
    printf("Error: invalid Panel returned by generator.\n");
    return NULL;
  }

  int num_groups = json_array_size(js_groups);

  for(int i = 0; i < num_groups; ++i) {
    Group* g = deserializeGroup(json_array_get(js_groups, i), p);
  }

  return NULL;
}

Group* Deserializer::deserializeGroup(json_t* el, Panel* panel) {
  
  if(!el) {
    printf("Error: trying to deserialize a group but the given json element is invalid.\n");
    return NULL;
  }

  json_t* js_widgets = json_object_get(el, "g");
  if(!json_is_array(js_widgets)) {
    printf("Error: no `g` element found in the group; no widgets.\n");
    return NULL;
  }

  Group* g = NULL;

  if(panel) {
    g = gen->createGroup(panel, "Dummy");
  }
  else {
    g = gen->createGroup("Dummy");
  }

  if(!g) {
    printf("Error: trying to create a group but the generator returned an invalid group.\n");
    return NULL;
  }
  
  int num_els = json_array_size(js_widgets);

  printf(">> %d\n", num_els);

  for(int i = 0; i < num_els; ++i) {

    json_t* js_widget = json_array_get(js_widgets, i);
    if(!json_is_object(js_widget)) {
      printf("Error: it seems that the widget of a group is invalid.\n");
      continue;
    }

    int type = 0;
    if(!remoxly_json_get_int(js_widget, "t", type)) {
      continue;
    }

    int id = 0;
    if(!remoxly_json_get_int(js_widget, "i", id)) {
      continue;
    }

    std::string label;
    if(!remoxly_json_get_string(js_widget, "l", label)) {
      continue;
    }

    switch(type) {

      case GUI_TYPE_SLIDER_INT: {
        Slider<int>* slider = deserializeSliderInt(js_widget, label, id);
        if(slider) {
          slider->id = id;
          g->add(slider);
        }
        break;
      }

      case GUI_TYPE_SLIDER_FLOAT: { 
        Slider<float>* slider = deserializeSliderFloat(js_widget, label, id);
        if(slider) {
          slider->id = id;
          g->add(slider);
        }
        break;
      }

      case GUI_TYPE_TOGGLE: { 
        Toggle* toggle = deserializeToggle(js_widget, label, id);
        if(toggle) {
          toggle->id = id;
          g->add(toggle);
        }
        break;
      }

      default: {
        printf("Warning: unhandled widget type in deserializer: %d for label: %s\n", type, label.c_str());
        break;
      }
    };
  }

  return g;
}

Slider<int>* Deserializer::deserializeSliderInt(json_t* el, std::string label, int id) {

  if(!el) {
    printf("Error: cannot deserialize slider, invalid json_t given.\n");
    return NULL;
  }

  int minv = 0;
  int maxv = 0;
  int step = 0;

  remoxly_json_get_int(el, "minv", minv);
  remoxly_json_get_int(el, "maxv", maxv);
  remoxly_json_get_int(el, "step", step);
  
  return gen->createSliderInt(label, minv, maxv, step, id);
}

Slider<float>* Deserializer::deserializeSliderFloat(json_t* el, std::string label, int id) {

  if(!el) {
    printf("Error: cannot deserialize slider, invalid json_t given.\n");
    return NULL;
  }

  float minv = 0.0f;
  float maxv = 0.0f;
  float step = 0.0f;

  remoxly_json_get_float(el, "minv", minv);
  remoxly_json_get_float(el, "maxv", maxv);
  remoxly_json_get_float(el, "step", step);

  return gen->createSliderFloat(label, minv, maxv, step, id);
}

Toggle* Deserializer::deserializeToggle(json_t* el, std::string label, int id) {
  
  if(!el) {
    printf("Error: cannot deserialize toggle; invalid json_t given.\n");
    return NULL;
  }

  return gen->createToggle(label, id);
}

bool Deserializer::deserializeTask(char* data, int& appID, int& taskID, std::string& value) {

  json_error_t err;
  json_t* root = json_loads((const char*)data, 0, &err);
  json_t* el_v = NULL;

  appID = 0;
  taskID = 0;

  if(!root) {
    printf("Error: cannot parse the received json: `%s`. Error: %s.\n", data, err.text);
    REMOXLY_FREE_JSON(root);
    return false;
  }

  if(!remoxly_json_get_int(root, "t", taskID)) {
    printf("Error: no `t` element found in the received data. Or the `t` element is not numeric: `%s`\n", data);
    REMOXLY_FREE_JSON(root);
    return false;
  }

  if(!remoxly_json_get_int(root, "i", appID)) {
    printf("Error: no `i` element found in received data. Or the `i` element is not numeric: `%s`\n", data);
    REMOXLY_FREE_JSON(root);
    return false;
  }

  json_t* js_value = json_object_get(root, "v");
  if(js_value) {
    value = json_dumps(js_value, 0);
  }

  REMOXLY_FREE_JSON(root);

  return true;
}

bool Deserializer::deserializeChangedValue(Widget* w, std::string& json) {

  json_error_t err;
  json_t* js_changed = json_loads(json.c_str(), 0, &err);

  if(!js_changed) {
    printf("ERROR: cannot parse changed value in Deserializer: %s\n", json.c_str());
    REMOXLY_FREE_JSON(js_changed);
    return false;
  }

  switch(w->type) {

    case GUI_TYPE_SLIDER_FLOAT: {

      float v = 0.0f;
      if(!remoxly_json_get_float(js_changed, "v", v)) {
        printf("Error: cannot get float value from the changed value.\n");
        REMOXLY_FREE_JSON(js_changed);
        return false;
      }
      
      Slider<float>* slider = static_cast<Slider<float>* >(w);
      slider->disableNotifications();
      slider->setAbsoluteValue(v);
      slider->enableNotifications();
      slider->needs_redraw = true;
      break;
    }

    case GUI_TYPE_SLIDER_INT: { 

      int v = 0;
      if(!remoxly_json_get_int(js_changed, "v", v)) {
        printf("Error: cannot get int value from the changed value.\n");
        REMOXLY_FREE_JSON(js_changed);
        return false;
      }
      
      Slider<int>* slider = static_cast<Slider<int>* >(w);
      slider->disableNotifications();
      slider->setAbsoluteValue(v);
      slider->enableNotifications();
      slider->needs_redraw = true;
      break;
    }

    default: {
      printf("Error: cannot deserialize a change value for widget: %s\n", w->label.c_str());
      break;
    }
  }

  REMOXLY_FREE_JSON(js_changed);

  return true;
}
