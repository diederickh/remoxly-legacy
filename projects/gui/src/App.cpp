#include <stdio.h>
#include <stdlib.h>
#include "App.h"

App::App() 
  :container(NULL)
{
}

App::~App() {
}

int App::init() {
  printf("App::init().\n");
  container = new Container(new RenderGL());
  container->add(new Toggle("Toggle 0", flag0)).setPosition(10, 10);
  container->add(new Toggle("Toggle 1", flag1)).setPosition(10, 40);
  container->add(new Toggle("Toggle 2", flag2)).setPosition(10, 70);
  container->add(new Toggle("Toggle 3", flag3)).setPosition(10, 100);
  return 0;
}

void App::draw() {
  container->draw();
}
