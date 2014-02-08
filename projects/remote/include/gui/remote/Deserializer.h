/*
 
  Deserializer
  ------------

  The Deserializer is used to deserialize a json string 
  which represents a gui. 


 */
#ifndef REMOXLY_GUI_DESERIALIZER_H
#define REMOXLY_GUI_DESERIALIZER_H

#include <vector>
#include <string>

extern "C" { 
#  include <jansson.h>
}

class Generator;
class Group;
class Panel;
class Toggle;
class Widget;
template<class T> class Slider;

class Deserializer {

 public:
  Deserializer(Generator* gen = NULL);
  bool deserialize(const std::string& model);                                      /* creates the panels, groups etc.. for the given string */

  /* deserialize widgets */
  Panel*         deserializePanel(json_t* el);                                     /* deserializes a panel with all groups and panels */
  Group*         deserializeGroup(json_t* el, Panel* p = NULL);                    /* deserializes a group with all of the sub widgets */
  Slider<int>*   deserializeSliderInt(json_t* el, std::string label, int id);      /* deserializes a slider<int> */
  Slider<float>* deserializeSliderFloat(json_t* el, std::string label, int id);    /* deserializes a slider<float> */
  Toggle*        deserializeToggle(json_t* el, std::string label, int id);         /* deserializes a toggle */

  /* protocol */
  bool deserializeTask(char* data, int& appID, int& taskID, std::string& value);   /* deserializes a task that we receive from the server; it merely extracts the app id and task id */
  bool deserializeChangedValue(Widget* w, std::string& json);                      /* deserialize the given json that contains values for the Widget */

 public:
  Generator* gen;
};

#endif

