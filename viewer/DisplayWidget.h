#pragma once
#include <QWidget>
#include <QImage>
#include <QPaintEvent>

class DisplayWidget
: public QWidget
{
public:
    DisplayWidget(QWidget* parent);
    void setImage(QImage image);

    void paintEvent(QPaintEvent* event);
private:
    QImage mImage;
};
