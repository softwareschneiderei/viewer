#include "FunctionEvent.h"
#include <cassert>
#include <QApplication>

QEvent::Type FunctionEvent::type()
{
    static QEvent::Type result = static_cast<QEvent::Type>(QEvent::registerEventType());
    return result;
}

FunctionEvent::FunctionEvent(FunctionEvent::Callback callback)
    : QEvent(type()), mCallback(callback)
{

}

FunctionEvent::~FunctionEvent()
{

}

bool FunctionEvent::dispatch(QEvent* event)
{
    assert(event!=nullptr);

    if (event->type() != FunctionEvent::type())
        return false;

    try
    {
        static_cast<FunctionEvent*>(event)->execute();
    }
    catch (...)
    {
    }
    return true;
}

void FunctionEvent::execute()
{
    mCallback();
}

void FunctionEvent::post(FunctionEvent::Callback callback)
{
    QApplication::postEvent(QApplication::instance(), new FunctionEvent(callback));
}
