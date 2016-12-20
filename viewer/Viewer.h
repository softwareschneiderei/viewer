#pragma once
#include <QMainWindow>
#include <memory>
#include "ImagePoller.h"

namespace Ui {
class Viewer;
}



class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

private:
    std::unique_ptr<Ui::Viewer> mUi;
    ImagePoller mPoller;
};

