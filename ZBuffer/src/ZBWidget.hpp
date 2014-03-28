#ifndef __ZB_WIDGET_HPP__
#define __ZB_WIDGET_HPP__

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>
#include <QResizeEvent>
#include "Model.hpp"

class ZBWidget : public QLabel {

Q_OBJECT

public:
  ZBWidget(Model *model, QWidget *parent=0);
  virtual ~ZBWidget();

protected:
  virtual void resizeEvent(QResizeEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  void render();

private:
  Model *m_model;
  QImage m_image;
  QPoint m_lastPos;
  float m_cameraAngleX;
  float m_cameraAngleY;
  float m_cameraDistance;

};

#endif //__ZB_WIDGET_HPP__
