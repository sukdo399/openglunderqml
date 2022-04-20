QT += qml quick
lessThan(QT_MAJOR_VERSION, 6) {
    QT += webengine
} else {
    QT += webenginequick
}

CONFIG += qmltypes

QML_IMPORT_NAME = OpenGLUnderQML
QML_IMPORT_MAJOR_VERSION = 1

HEADERS += \
    opengltest.h
SOURCES += main.cpp \
    opengltest.cpp
RESOURCES += openglunderqml.qrc

DISTFILES +=
