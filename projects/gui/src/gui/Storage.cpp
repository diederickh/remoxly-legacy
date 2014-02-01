#include <gui/Storage.h>
#include <gui/Gui.h>
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

  for(std::vector<Gui*>::iterator it = p->guis.begin(); it != p->guis.end(); ++it) {
    addGui(*it);
  }
}

void Storage::addGui(Gui* g) {

  if(!g) {
    return;
  }

  guis.push_back(g);
}



