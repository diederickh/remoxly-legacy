#include <gui/Render.h>

bool Render::setup() {  return false; }

void Render::update() { }

void Render::draw() { }

void Render::getWindowSize(int& ww, int& wh) { }

void Render::beginScissor() { } 

void Render::scissor(int sx, int sy, int sw, int sh) { }

void Render::endScissor() { } 

void Render::clear() { }

void Render::writeText(float x, float y, std::string text, float* color) { } 

void Render::writeNumber(float x, float y, std::string number, float* color) { } 

void Render::writeIcon(float x, float y, unsigned int icon, float* color) { }

void Render::enableTextInput(float x, float y, float maxw, std::string value, float* color) { }

bool Render::getIconSize(unsigned int id, int& w, int& h) { w = 0; h = 0; return false; } 

void Render::getNumberInputValue(std::string& result) { }

void Render::disableNumberInput() { }

void Render::enableNumberInput(float x, float y, float maxw, std::string value, float* color) { }

void Render::addRectangle(float x, float y, float w, float h, float* color, bool filled) { }

void Render::onCharPress(unsigned int key) { }

void Render::onKeyPress(int key, int mods) { }


