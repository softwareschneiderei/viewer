#pragma once
#include <QEvent>
#include <functional>

class FunctionEvent
: public QEvent
{
public:
    static QEvent::Type type();

    using Callback=std::function<void()>;

    explicit FunctionEvent(Callback callback);

    ~FunctionEvent();

    static bool dispatch(QEvent* event);

    void execute();

    static void post(Callback callback);



private:
    Callback mCallback;
};
