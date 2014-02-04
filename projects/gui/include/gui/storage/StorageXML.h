/*

  StorageXML
  ----------

  This storage type will save the added guis/panels into a XML file. 
  
  We use rapidxml for loading the xml.

 */
#ifndef REMOXLY_GUI_STORAGE_XML_H
#define REMOXLY_GUI_STORAGE_XML_H

#include <gui/Storage.h>
#include <fstream>
#include <sstream>

class StorageXML : public Storage {

 public:
  StorageXML(std::string filepath);                  /* pass in the filepath where you want to save/load the xml */
  bool save();                                       /* save the added panels/gui into the xml */
  bool load();                                       /* load previously saved settings from the xml */

  Group* findGroup(std::string name);                    /* internally used to find the gui by the given (cleaned up) name */
  Widget* findWidget(Group* g, std::string name);      /* find a widget with the given (cleaned up) name */

 public:
  std::string filepath;                              /* filepath where we save/load data into/from */
};

#endif
