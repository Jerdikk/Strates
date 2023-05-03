#include "application.h"
#include "config.h"

FILE* logfile;

int main(int argc, char *argv[]) {
	Open_Error_File("prog.log", logfile);
  Application *app = new Application();
  app->run();
  delete app;
  return 0;
}
