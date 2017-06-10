import QtQuick 2.0

Rectangle {
    id: background
    color: "transparent"
    anchors.centerIn: parent
    property alias backgroundGridViewModel: gridView.model

    GridView {
        id: gridView

        anchors.centerIn: parent
        anchors.fill: parent

        cellWidth: parent.width / itemsInRow
        cellHeight: parent.width / itemsInRow

        delegate:
            Rectangle {
            radius: 2
            width: gridView.cellWidth - 2
            height: gridView.cellHeight - 2

        }
    }
}
