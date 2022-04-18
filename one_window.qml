import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10
import OpenGLUnderQML 1.0

Window {
    id: rootWindow
    visible: true
    color: "transparent"
    width: 1280
    height: 720
    x: 0
    y: 0

    OpenglTest {
//        SequentialAnimation on t {
//            NumberAnimation {
//                to: 360
//                duration: 3000
//            }
//            loops: Animation.Infinite
//            running: true
//        }
    }

    Timer {
        id: timer
        running: true
        repeat: true
        interval: 17
        onTriggered: {
            rootWindow.update()
        }
    }

    WebEngineView {
        anchors.fill: parent
        anchors.bottom: parent.bottom
        backgroundColor: "transparent"
        url: "qrc:/main.html"
    }
}
