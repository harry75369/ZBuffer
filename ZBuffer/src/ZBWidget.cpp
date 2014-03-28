#include <QSizePolicy>
#include "ZBWidget.hpp"
#include "Logger.hpp"

ZBWidget::ZBWidget(Model *model, QWidget *parent)
  : QLabel(parent),
    m_model(model),
    m_cameraAngleX(0.0f),
    m_cameraAngleY(0.0f),
    m_cameraDistance(10.f)
{
  QImage img;
  img.load("logo.png");
  setPixmap(QPixmap::fromImage(img));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setScaledContents(true);
}

ZBWidget::~ZBWidget()
{
}

void ZBWidget::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size();
  INFO("size = (%d, %d)", size.width(), size.height());
}

void ZBWidget::mouseMoveEvent(QMouseEvent *event)
{
  const QPoint &pos = event->pos();
  INFO("pos = (%d, %d)", pos.x(), pos.y());

  if ( event->buttons() & Qt::LeftButton )
  {
    m_cameraAngleY += (pos.x()-m_lastPos.x());
    m_cameraAngleX += (pos.y()-m_lastPos.y());
  }
  else if ( event->buttons() & Qt::RightButton )
  {
    m_cameraDistance -= (pos.y()-m_lastPos.y()) * 0.2f;
  }
  m_lastPos = event->pos();
}

void ZBWidget::mousePressEvent(QMouseEvent *event)
{
  m_lastPos = event->pos();
}

void ZBWidget::render()
{
}
