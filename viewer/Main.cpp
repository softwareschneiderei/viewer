#include <QApplication>
#include "Viewer.h"
#include "Main.h"


int main(int argc, char** argv)
{
    Application application(argc, argv);
    Application::setApplicationName("Viewer");

    Viewer window;
    window.show();

    application.exec();
}
