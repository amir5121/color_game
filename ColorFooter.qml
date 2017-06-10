import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Rectangle {
    width: gridView.width
    property alias takeOverBtn: takeOverButton
    property alias resetBtn: resetButton
    Row {
        anchors.fill: parent
        spacing: 5
        Button {
            id: takeOverButton
            width: parent.width / 2 - 5

            highlighted: true
            Material.accent: !takenOver ? "#FFFF00": "#FFFFFF"

            Text {
                text: !takenOver ? "Take Over!" : "Stop!"
                anchors.centerIn: parent
                color: !takenOver ? "#37474F" : "#b71c1c"
            }
        }

        Button {
            id: resetButton
            width: parent.width / 2 - 5

            highlighted: true
            Material.accent: "#4A148C"

            Text {
                text: "Reset"
                anchors.centerIn: parent
                color: "#FFFFFF"
            }
        }
    }

}
