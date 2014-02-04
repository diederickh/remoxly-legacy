/*

  Storage 
  -------

  The Storage class is a base class for storage implementations. By 
  default we provide a XML storage class that uses RapidXML. 

 */
#ifndef REMOXLY_GUI_STORAGE_H
#define REMOXLY_GUI_STORAGE_H

#include <vector>

class Group;
class Panel;

class Storage {

 public:
  virtual bool save();
  virtual bool load();
  void addPanel(Panel* p);
  void addGroup(Group* g);

 public:
  std::vector<Group*> groups;
};

#endif
