#include "Viewer.h"
#include "ui_Viewer.h"
#include <QToolButton>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::Viewer),
    mPoller("HELGETEST1-CCDCAM:", 21721600)
{
    mUi->setupUi(this);
    mImageStatsLabel = new QLabel(this);
    statusBar()->addPermanentWidget(mImageStatsLabel);

    connect(mUi->tonemapping, &QToolButton::clicked, [this](bool checked)
    {
        mPoller.setTonemapping(checked);
    });

    mPoller.start([this](ImagePoller::Result result)
    {
        updateImage(result);
    });
}

Viewer::~Viewer()
{
    mPoller.stop();
}

void Viewer::updateImage(ImagePoller::Result result)
{
    QPixmap pixmap;
    pixmap.convertFromImage(result.image);
    mUi->label->setPixmap(pixmap);
    mImageStatsLabel->setText(QString("Min: %1, Max %2").arg(result.min).arg(result.max));
}
