#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>

int main(int argc, char *argv[])
{
    QtWebEngine::initialize();
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
