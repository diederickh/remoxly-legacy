#include <stdio.h>
#include <stdlib.h>

#include <gui/remote/Server.h>
#include <signal.h>

void sighandler(int sig);
bool must_run = true;

int main() {
  printf("Remoxly Standalone Server (20140213).\n");

  signal(SIGINT, sighandler);

  Server server(2255, false);
  server.start();

  while(must_run) {
     server.update();     
  }

  return EXIT_SUCCESS;
}

void sighandler(int sig) {
  must_run = false;
}

