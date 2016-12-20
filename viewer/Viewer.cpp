#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::Viewer),
    mPoller([]{})
{
    mUi->setupUi(this);
    mPoller.start();
}

Viewer::~Viewer()
{
    mPoller.stop();
}
