#include "DisplayWidget.h"
#include <QPainter>

void DisplayWidget::setImage(QImage image)
{
  mImage = image;
  update();
}

DisplayWidget::DisplayWidget(QWidget* parent)
  : QWidget(parent)
{

}

void DisplayWidget::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  if (mImage.isNull())
  {
    painter.drawText(rect(), Qt::AlignCenter, "Press start to see the live image");
    return;
  }

  QSize size = event->rect().size();
  QImage current = mImage.scaled(size, Qt::KeepAspectRatio);

  painter.drawImage((size.width() - current.width()) / 2, (size.height() - current.height()) / 2, current);
}
