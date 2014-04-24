#include <stdio.h>
#include <QApplication>
#include "MainWindow.hpp"
#include <QGLFormat>

int main(int argc, char * argv[]) {
  if ( argc != 2 ) {
    printf("Usage: zbuffer obj_file\n");
    return 1;
  }

  QApplication app(argc, argv);

  QGLFormat glf = QGLFormat::defaultFormat();
  glf.setSampleBuffers(true);
  glf.setSamples(4);
  QGLFormat::setDefaultFormat(glf);

  MainWindow window(new Model(argv[1]));
  window.show();

  return app.exec();
}
