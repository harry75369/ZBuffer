#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "GLWidget.hpp"

MainWindow::MainWindow(Model *model, QWidget *parent) :
  QMainWindow(parent),
  m_model(model),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_ui->horizontalLayout->addWidget(new GLWidget(model, this));
}

MainWindow::~MainWindow()
{
  delete m_model;
  delete m_ui;
}
