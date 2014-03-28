#include <stdio.h>
#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char * argv[]) {
  if ( argc != 2 ) {
    printf("Usage: zbuffer obj_file\n");
    return 1;
  }

  QApplication app(argc, argv);
  MainWindow window(new Model(argv[1]));
  window.show();

  return app.exec();
}
