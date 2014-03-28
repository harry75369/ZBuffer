SOURCES += \
  lib/glew.c \
  lib/tiny_obj_loader.cc \
  src/MainWindow.cpp \
  src/GLWidget.cpp \
  src/ZBWidget.cpp \
  src/Model.cpp \
  src/main.cc

HEADERS += \
  src/GLWidget.hpp \
  src/MainWindow.hpp \
  src/ZBWidget.hpp

FORMS += \
  src/MainWindow.ui

INCLUDEPATH += lib/ \
  ../QGLViewer/ \
  ../

DEPENDPATH += lib/ \
  ../QGLViewer/ \
  ../

LIBS += -L../QGLViewer \
  -lQGLViewer \
  -lGLU

OBJECTS_DIR = build/
RCC_DIR     = build/
MOC_DIR     = build/
UI_DIR      = build/

QT          += opengl xml widgets gui
CONFIG      += debug

DESTDIR     = ..
TARGET      = zbuffer
