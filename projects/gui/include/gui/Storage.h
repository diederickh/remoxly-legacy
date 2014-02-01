#ifndef REMOXLY_STORAGE_H
#define REMOXLY_STORAGE_H

#include <vector>

class Gui;
class Panel;

class Storage {

 public:
  virtual bool save();
  virtual bool load();
  void addPanel(Panel* p);
  void addGui(Gui* g);

 public:
  std::vector<Gui*> guis;
};

#endif
