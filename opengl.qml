import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10
import OpenGLUnderQML 1.0

Window {
    id: rootWindow
    title: "OPENGL"
    visible: true
    width: 200
    height: 200
    x: 200
    y: 200

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

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "blue"
    }
}
