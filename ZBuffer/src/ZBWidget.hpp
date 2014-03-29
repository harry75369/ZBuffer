#ifndef __ZB_WIDGET_HPP__
#define __ZB_WIDGET_HPP__

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <Eigen/Eigen>
#include "Model.hpp"

class ZBWidget : public QWidget {

Q_OBJECT

public:
  typedef Eigen::Matrix4d Matrix4;
  typedef Eigen::Vector3d Vector3;

public:
  ZBWidget(Model *model, QWidget *parent=0);
  virtual ~ZBWidget();

public:
  static Matrix4 lookAt(float eye_x, float eye_y, float eye_z,
                        float center_x, float center_y, float center_z,
                        float up_x, float up_y, float up_z);
  static Matrix4 perspective(float fov, float aspect, float near, float far);
  static Matrix4 rotateX(float degree);
  static Matrix4 rotateY(float degree);

protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);

signals:
  void repaintNeeded();

private:
  Model *m_model;
  QPoint m_lastPos;
  float m_cameraAngleX;
  float m_cameraAngleY;
  float m_cameraDistance;

};

#endif //__ZB_WIDGET_HPP__
