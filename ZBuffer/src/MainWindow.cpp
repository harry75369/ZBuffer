#include <QLabel>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "GLWidget.hpp"
#include "ZBWidget.hpp"

MainWindow::MainWindow(Model *model, QWidget *parent) :
  QMainWindow(parent),
  m_model(model),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_ui->gridLayout->addWidget(new QLabel("OpenGL"), 0, 0, Qt::AlignCenter);
  m_ui->gridLayout->addWidget(new GLWidget(model, this), 1, 0);
  m_ui->gridLayout->addWidget(new QLabel("ZBuffer"), 0, 1, Qt::AlignCenter);
  m_ui->gridLayout->addWidget(new ZBWidget(model, this), 1, 1);
  m_ui->gridLayout->setColumnStretch(0, 1);
  m_ui->gridLayout->setColumnStretch(1, 1);
  m_ui->gridLayout->setRowStretch(0, 0);
  m_ui->gridLayout->setRowStretch(1, 1);
}

MainWindow::~MainWindow()
{
  delete m_model;
  delete m_ui;
}
