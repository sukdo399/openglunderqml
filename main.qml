import QtQuick 2.15
import QtQuick.Window 2.15
import QtWebEngine 1.10

Window {
    visible: true
    title: "HTML5"
    color: "transparent"
    width: 1920
    height: 720
    x: 0
    y: 0

    WebEngineView {
        anchors.fill: parent
        anchors.bottom: parent.bottom
        backgroundColor: "transparent"
        url: "qrc:/main.html"
//        url: "https://www.youtube.com"
    }
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "red"
    }
}
