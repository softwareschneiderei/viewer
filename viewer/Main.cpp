#include <QApplication>
#include <QCommandLineParser>
#include "Viewer.h"
#include "Main.h"
//#include "Device.h"
#include <iostream>
#include <QMessageBox>

#ifndef VIEWER_VERSION
# error VIEWER_VERSION needs to be provided by the build system
#endif

#define QUOTE(x) #x
#define EXPAND_QUOTE(x) QUOTE(x)

int main(int argc, char** argv)
{
  // About enough memory for a 1280x1024
  setenv("EPICS_CA_MAX_ARRAY_BYTES", "3000000", true);


  Application application(argc, argv);
  Application::setApplicationName("Viewer");
  Application::setApplicationVersion(EXPAND_QUOTE(VIEWER_VERSION));

  QCommandLineParser parser;

  parser.addHelpOption();
  parser.addVersionOption();

  parser.process(application);


  Viewer window;
  window.show();

  application.exec();

}
