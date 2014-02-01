#include <gui/Utils.h>
#include <gui/Widget.h>
#include <gui/Slider.h>
#include <gui/Toggle.h>
#include <gui/storage/StorageXML.h>
#include <rapidxml.hpp>

#define REMOXLY_XML_CHECK(n,s) { if(!n) { printf("%s", s); return false; } } 

using namespace rapidxml;

StorageXML::StorageXML(std::string filepath)
  :filepath(filepath)
{
}

bool StorageXML::save() {

  std::stringstream ss;
  std::string name;

  ss << "<settings>\n";

  for(std::vector<Gui*>::iterator it = guis.begin(); it != guis.end(); ++it) {

    Gui* g = *it;
    name = gui_cleanup_string(g->label);
    
    ss << "  <gui name=\"" << name.c_str() << "\">\n";

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

    ss << "  </gui>\n";
  }
  ss << "</settings>\n";

  std::string xml = ss.str();

  if(!xml.size()) {
    printf("Warning: trying to save a gui/panel, but we didn't find any elements to serialize.\n");
    return false;
  }

  std::ofstream ofs(filepath.c_str());
  if(!ofs.is_open()) {
    printf("Error: cannot open the xml file to save gui settings: `%s`.\n", filepath.c_str());
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

    xml_node<>* xgui = xroot->first_node("gui");    
    REMOXLY_XML_CHECK(xgui, "Error: Cannot find any GUIs in the loaded xml.\n");

    while(xgui) {

      // find the gui
      xml_attribute<>* xgui_name = xgui->first_attribute("name"); 
      REMOXLY_XML_CHECK(xgui_name, "Error: gui element does not have a name field.\n");

      std::string gui_name = xgui_name->value();

      Gui* gui = findGui(gui_name);
      if(!gui) {
        printf("Error: cannot find the gui: %s\n", gui_name.c_str());
        xgui = xgui->next_sibling();
        continue;
      }

      // iterate over all widgets in gui
      xml_node<>* xwidget = xgui->first_node("widget"); 
      if(!xwidget) {
        printf("Error: it seems that the gui `%s` does not have any widgets that are saved.\n", gui_name.c_str());
        xgui = xgui->next_sibling();
        continue;
      }

      while(xwidget) {

        xml_attribute<>* xwidget_name = xwidget->first_attribute("name"); REMOXLY_XML_CHECK(xwidget_name, "Error: cannot find the name for the current widget. Wrong XML.\n");
        std::string widget_name = xwidget_name->value();
        Widget* widget = findWidget(gui, widget_name);

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

      xgui = xgui->next_sibling();
    }

  }
  catch(...) {
    printf("Error: error while parsing the xml.\n");
    return false;
  }

  return true;
}

Gui* StorageXML::findGui(std::string name) {

  for(std::vector<Gui*>::iterator it = guis.begin(); it != guis.end(); ++it) {
    Gui* g = *it;
    std::string gui_name = gui_cleanup_string(g->label);
    if(gui_name == name) {
      return g;
    }
  }
  return NULL;
}

Widget* StorageXML::findWidget(Gui* g, std::string name) {
  
  for(std::vector<Widget*>::iterator it = g->children.begin(); it != g->children.end(); ++it) {
    Widget* wid = *it;
    std::string widget_name = gui_cleanup_string(wid->label);
    if(widget_name == name) {
      return wid;
    }
  }
  return NULL;
}
