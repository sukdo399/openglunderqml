QT += qml quick webengine

CONFIG += qmltypes

QML_IMPORT_NAME = OpenGLUnderQML
QML_IMPORT_MAJOR_VERSION = 1

HEADERS += \
    opengltest.h
SOURCES += main.cpp \
    opengltest.cpp
RESOURCES += openglunderqml.qrc

DISTFILES +=
