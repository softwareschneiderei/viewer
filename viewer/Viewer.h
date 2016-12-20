#pragma once
#include <QMainWindow>
#include <memory>
#include "ImagePoller.h"
#include <QLabel>

namespace Ui {
class Viewer;
}



class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer(QString device, int serial, QWidget *parent = 0);
    ~Viewer();

private:
    void updateImage(ImagePoller::Result result);

    std::unique_ptr<Ui::Viewer> mUi;
    ImagePoller mPoller;
    QLabel* mImageStatsLabel;
};

