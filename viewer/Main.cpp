#include <QApplication>
#include "Viewer.h"
#include "Main.h"
#include "Device.h"


int main(int argc, char** argv)
{
    // About enough memory for a 1280x1024
    setenv("EPICS_CA_MAX_ARRAY_BYTES", "3000000", true);


    Application application(argc, argv);
    Application::setApplicationName("Viewer");

    Viewer window;
    window.show();

    application.exec();

}
