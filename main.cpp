#include <QGuiApplication>
#include <QQmlApplicationEngine>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <qtwebengineglobal.h>
#else
#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#endif
#include "opengltest.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QtWebEngine::initialize();
#else
    QtWebEngineQuick::initialize();
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
#if 1
    engine.load(QUrl(QStringLiteral("qrc:/opengl.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#else
    engine.load(QUrl(QStringLiteral("qrc:/one_window.qml")));
#endif
    return app.exec();
}
