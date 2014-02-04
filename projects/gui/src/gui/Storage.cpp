#include <gui/Storage.h>
#include <gui/Group.h>
#include <gui/Panel.h>

bool Storage::load() {
  return false;
}

bool Storage::save() {
  return false;
}

void Storage::addPanel(Panel* p) {

  if(!p) {
    return;
  }

  for(std::vector<Group*>::iterator it = p->groups.begin(); it != p->groups.end(); ++it) {
    addGroup(*it);
  }
}

void Storage::addGroup(Group* g) {

  if(!g) {
    return;
  }

  groups.push_back(g);
}



