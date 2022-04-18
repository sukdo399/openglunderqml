import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10
import OpenGLUnderQML 1.0

Window {
    visible: true
    width: 1280
    height: 720
    x: 50
    y: 50

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
}
