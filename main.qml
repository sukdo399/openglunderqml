import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10
import OpenGLUnderQML 1.0

Window {
    visible: true
    color: "transparent"
    width: 1280
    height: 720
    x: 0
    y: 0

    WebEngineView {
        anchors.fill: parent
        anchors.bottom: parent.bottom
        backgroundColor: "transparent"
        url: "qrc:/main.html"
    }
}
