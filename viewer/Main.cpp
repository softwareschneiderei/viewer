#include <QApplication>
#include <QCommandLineParser>
#include "Viewer.h"
#include "Main.h"
#include "Device.h"
#include <iostream>
#include <QMessageBox>


int main(int argc, char** argv)
{
    // About enough memory for a 1280x1024
    setenv("EPICS_CA_MAX_ARRAY_BYTES", "3000000", true);


    Application application(argc, argv);
    Application::setApplicationName("Viewer");

    QCommandLineParser parser;

    parser.addHelpOption();
    //parser.addVersionOption();


    QCommandLineOption deviceOption(QStringList() << "d" << "device",
                                        "Device prefix for the IOC server", "device-prefix");
    QCommandLineOption serialOption(QStringList() << "s" << "serial",
                                      "Serial number of the camera to open", "serial-number");

    parser.addOption(deviceOption);
    parser.addOption(serialOption);

    parser.process(application);

    QString device = parser.value(deviceOption);

    bool ok=true;
    QString serial = parser.value(serialOption);
    int serialValue = serial.toInt(&ok);

    /*if (!ok)
    {
        QMessageBox::critical(nullptr, "Invalid serial",
                          QString("The given serial %1 is not a valid number.").arg(serial));
        return -1;
    }*/


    Viewer window(device, serialValue);
    window.show();

    application.exec();

}
