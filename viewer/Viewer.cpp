#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::Viewer),
    mPoller()
{
    mUi->setupUi(this);
    mPoller.start([this](QImage image)
    {
        QPixmap pixmap;
        pixmap.convertFromImage(image);
        mUi->label->setPixmap(pixmap);
    });
}

Viewer::~Viewer()
{
    mPoller.stop();
}
