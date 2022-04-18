import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10
import OpenGLUnderQML 1.0

Window {
    visible: true
    width: 1280
    height: 720

    OpenglTest {
        SequentialAnimation on t {
            NumberAnimation {
                to: 360
                duration: 3000
            }
            loops: Animation.Infinite
            running: true
        }
    }

    WebEngineView {
        anchors.fill: parent
        anchors.bottom: parent.bottom
        backgroundColor: "transparent"
        url: "qrc:/main.html"
    }
}
