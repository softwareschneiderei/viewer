#pragma once
#include "FunctionEvent.h"

class Application : public QApplication
{
public:
    Application(int& argn, char** argv) : QApplication(argn, argv) {}

    void customEvent(QEvent* event) override
    {
        if (FunctionEvent::dispatch(event))
            return;

        return QApplication::customEvent(event);
    }

    bool notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (...)
        {
            // TODO: log or something
        }

        return false;
    }
};
