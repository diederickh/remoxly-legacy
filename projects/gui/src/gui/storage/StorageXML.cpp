#include <gui/Utils.h>
#include <gui/Widget.h>
#include <gui/Slider.h>
#include <gui/Toggle.h>
#include <gui/Text.h>
#include <gui/ColorRGB.h>
#include <gui/storage/StorageXML.h>
#include <rapidxml.hpp>

#define REMOXLY_XML_CHECK(n,s) { if(!n) { printf("%s", s); return false; } } 

using namespace rapidxml;

namespace rx { 

StorageXML::StorageXML(std::string filepath)
  :filepath(filepath)
{
}

bool StorageXML::save() {

  std::stringstream ss;
  std::string name;

  ss << "<settings>\n";

  for(std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {

    Group* g = *it;
    name = gui_cleanup_string(g->label);
    
    ss << "  <group name=\"" << name.c_str() << "\">\n";

    for(std::vector<Widget*>::iterator wit = g->children.begin(); wit != g->children.end(); ++wit) {

      Widget* widget = *wit;
      name = gui_cleanup_string(widget->label);

      switch(widget->type) {

        case GUI_TYPE_SLIDER_INT: {
          Slider<int>* slider = static_cast<Slider<int>* >(widget);
          ss << "    <widget type=\"" << widget->type << "\" name=\"" << name.c_str() << "\">" << slider->value << "</widget>\n";
          break;
        }

        case GUI_TYPE_SLIDER_FLOAT: {
          Slider<float>* slider = static_cast<Slider<float>* >(widget);
          ss << "    <widget type=\"" << widget->type << "\" name=\"" << name.c_str() << "\">" << slider->value << "</widget>\n";
          break;
        }

        case GUI_TYPE_TOGGLE: {
          Toggle* toggle = static_cast<Toggle*>(widget);
          ss << "    <widget type=\"" << widget->type << "\" name=\"" << name.c_str() << "\">" << toggle->value << "</widget>\n";
          break;
        }

        case GUI_TYPE_COLOR_RGB: {
          ColorRGB* color = static_cast<ColorRGB*>(widget);
          ss << "    <widget type=\"" << widget->type << "\" name=\"" << name.c_str() << "\">" << color->perc_value << "</widget>\n";
          break;
        }

        case GUI_TYPE_TEXT: {
          Text* text = static_cast<Text*>(widget);
          ss << "    <widget type=\"" << widget->type << "\" name=\"" << name.c_str() << "\">" << text->value << "</widget>\n";
          break;
        }

        case GUI_TYPE_BUTTON: {
          break;
        }

        default: { 
          std::string clean = gui_cleanup_string(widget->label);
          printf("Unknown element, with name: %s, %s\n", widget->label.c_str(), clean.c_str());
          break;
        }
      } // switch

      name.clear();
    }

    ss << "  </group>\n";
  }
  ss << "</settings>\n";

  std::string xml = ss.str();

  if(!xml.size()) {
    printf("Warning: trying to save a group/panel, but we didn't find any elements to serialize.\n");
    return false;
  }

  std::ofstream ofs(filepath.c_str());
  if(!ofs.is_open()) {
    printf("Error: cannot open the xml file to save group settings: `%s`.\n", filepath.c_str());
    return false;
  }

  ofs << xml.c_str();
  ofs.flush();
  ofs.close();

  return true;
}

bool StorageXML::load() {
  
  std::ifstream ifs(filepath.c_str(), std::ios::in);

  if(!ifs.is_open()) {
    printf("Error: cannot load the xml file: `%s`.\n", filepath.c_str());
    return false;
  }

  std::string xml_str;
  xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

  if(!xml_str.size()) {
    printf("Error: xml file appears to be empty.\n");
    return false;
  }

  xml_document<> doc;

  try {

    doc.parse<0>((char*)xml_str.c_str());

    xml_node<>* xroot = doc.first_node("settings"); 
    REMOXLY_XML_CHECK(xroot, "Error: Cannot find the setting element.\n");

    xml_node<>* xgroup = xroot->first_node("group");    
    REMOXLY_XML_CHECK(xgroup, "Error: Cannot find any groups in the loaded xml.\n");

    while(xgroup) {

      // find the group
      xml_attribute<>* xgroup_name = xgroup->first_attribute("name"); 
      REMOXLY_XML_CHECK(xgroup_name, "Error: group element does not have a name field.\n");

      std::string group_name = xgroup_name->value();

      Group* group = findGroup(group_name);
      if(!group) {
        printf("Error: cannot find the group: %s\n", group_name.c_str());
        xgroup = xgroup->next_sibling();
        continue;
      }

      // iterate over all widgets in group
      xml_node<>* xwidget = xgroup->first_node("widget"); 
      if(!xwidget) {
        xgroup = xgroup->next_sibling();
        continue;
      }

      while(xwidget) {

        xml_attribute<>* xwidget_name = xwidget->first_attribute("name"); REMOXLY_XML_CHECK(xwidget_name, "Error: cannot find the name for the current widget. Wrong XML.\n");
        std::string widget_name = xwidget_name->value();
        Widget* widget = findWidget(group, widget_name);

        if(!widget) {
          printf("Error: cannot find the widget: %s\n", widget_name.c_str());
          xwidget = xwidget->next_sibling();
          continue;
        }

        switch(widget->type) {

          case GUI_TYPE_SLIDER_INT: {
            Slider<int>* slider = static_cast<Slider<int>* >(widget);
            std::string str_value = xwidget->value();
            slider->setAbsoluteValue(gui_string_to_int(str_value));
            slider->needs_redraw = true;
            break;
          }

          case GUI_TYPE_SLIDER_FLOAT: {
            Slider<float>* slider = static_cast<Slider<float>* >(widget);
            std::string str_value = xwidget->value();
            slider->setAbsoluteValue(gui_string_to_float(str_value));
            slider->needs_redraw = true;
            break;
          }

          case GUI_TYPE_TOGGLE: {
            Toggle* toggle = static_cast<Toggle*>(widget);
            std::string str_value = xwidget->value();
            toggle->value = gui_string_to_bool(str_value);
            toggle->needs_redraw = true;
            break;
          }

          case GUI_TYPE_COLOR_RGB: {
            ColorRGB* color = static_cast<ColorRGB*>(widget);
            std::string str_value = xwidget->value();
            color->setPercentageValue(gui_string_to_float(str_value));
            color->needs_redraw = true;
            break;
          }

          case GUI_TYPE_TEXT: {
            Text* text = static_cast<Text*>(widget);
            text->value = xwidget->value();
            text->needs_redraw = true;
            break;
          }

          case GUI_TYPE_BUTTON: {
            break;
          }

          default: {
            printf("Error: we have not yet implemented loading of the widget for type %d\n", widget->type);
            break;
          }
        }

        xwidget = xwidget->next_sibling();
      }

      xgroup = xgroup->next_sibling();
    }

  }
  catch(...) {
    printf("Error: error while parsing the xml.\n");
    return false;
  }

  return true;
}

Group* StorageXML::findGroup(std::string name) {

  for(std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {
    Group* g = *it;
    std::string group_name = gui_cleanup_string(g->label);
    if(group_name == name) {
      return g;
    }
  }
  return NULL;
}

Widget* StorageXML::findWidget(Group* g, std::string name) {
  
  for(std::vector<Widget*>::iterator it = g->children.begin(); it != g->children.end(); ++it) {
    Widget* wid = *it;
    std::string widget_name = gui_cleanup_string(wid->label);
    if(widget_name == name) {
      return wid;
    }
  }
  return NULL;
}

} // namespace rx
