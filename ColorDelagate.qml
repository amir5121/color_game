import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    property alias mouseArea: mMouseArea
    property alias colorAnim: colorAnim

    width: gridView.cellWidth - 4
    height: gridView.cellHeight - 4
    radius: 3

    layer.enabled: true
    layer.effect: DropShadow {
        color: "#66000000"
        radius: 5
    }

    MouseArea {
        id: mMouseArea
        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        text: score
        font {
            pointSize:  parent.width / 5
        }
        color: "white"
    }


    ColorAnimation on color{
        id: colorAnim
        from: "white"
        to: m_color
    }

    onColorChanged: {
        if (!colorAnim.running) {
            colorAnim.restart()
        }
    }

    color: m_color
}
