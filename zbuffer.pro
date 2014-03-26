CONFIG       += ordered
TEMPLATE      = subdirs
SUBDIRS       = QGLViewer ZBuffer

QT_VERSION=$$[QT_VERSION]

contains( QT_VERSION, "^5.*" ) {
	cache()
}
