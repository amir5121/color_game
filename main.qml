import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0


ApplicationWindow {
    id: root
    visible: true
    height: 600
    width: 800
    x: 250
    y: 50
    color: "#BBDEFB"
    title: qsTr("8 puzzel")

    property int itemCount: 64
    property int defaultScore: 5
    property string redPlayerColor: "#ef5350"
    property string greenPlayerColor: "#26A69A"
    property int speed: 100
    property int itemsInRow: Math.sqrt(itemCount)
    property bool redTurn: true
    property var buttonAnimQueue;
    property bool delagateEnabled: true
    property bool takenOver: false
    property var clickQueue;


    //TODO: add invalid state to the game FUCK you Bitch in the ass so hard you'd never forget ever... LOVE ;)

    Component.onCompleted: {
        setUp(itemsInRow)
    }

    function setUp(width) {
        if (width < 5) width = 5;
        if (width > 12) width = 12;
        itemCount = width * width;
        colorModel.clear();
        root.color = "#BBDEFB"
        for (var i = 0; i < itemCount; i++) {
            if (i == 0 || i == itemCount - 1 ) {
                colorModel.append({"score": defaultScore, "m_color": String(Qt.darker(redPlayerColor , getAmount(defaultScore))), "colorState": 1})
                //            colorModel.append({"score": i, "m_color": String(Qt.darker(redPlayerColor , getAmount(defaultScore))), "colorState": 1})

            } else if (i == itemsInRow - 1 ||i == (itemCount -  itemsInRow)) {
                colorModel.append({"score": defaultScore, "m_color": String(Qt.darker(greenPlayerColor , getAmount(defaultScore))),  "colorState": 2})
            }

            else {
                colorModel.append({"score": 0, "m_color": "#FFFFFF",  "colorState": 0})
            }
        }

        Controller.init(redTurn, itemCount);

        buttonAnimQueue = new Array;
        clickQueue = new Array
        takenOver = false
        redTurn = true
        colorAnim.restart()
        updateScores()
        gridView.focus = true
    }


    Rectangle {
        id: gridContainer
        width: ((parent.height < parent.width) ? parent.height : parent.width) * .8
        height: ((parent.height < parent.width) ? parent.height : parent.width) * .9

        ColorAnimation on color{
            id: colorAnim
            from: redTurn ? greenPlayerColor : redPlayerColor
            to: redTurn ? redPlayerColor : greenPlayerColor
            duration: speed * 5
        }

        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: parent.width / 25
        anchors.left: parent.left
        radius: 5

        layer.enabled: true
        layer.effect: DropShadow {
            color: "#66000000"
            radius: 5
        }

        GridView {
            id: gridView
            width: parent.width - 25
            height: parent.height - 25

            Keys.onDigit5Pressed: setUp(5)
            Keys.onDigit6Pressed: setUp(6)
            Keys.onDigit7Pressed: setUp(7)
            Keys.onDigit8Pressed: setUp(8)
            Keys.onDigit9Pressed: setUp(9)

            Keys.onUpPressed: setUp(itemsInRow + 1)
            Keys.onDownPressed: setUp(itemsInRow - 1)

            interactive: false

            cellWidth: (width - 1) / itemsInRow
            cellHeight: (width - 1) / itemsInRow

            anchors.centerIn: parent

            footer: ColorFooter {
                takeOverBtn.onClicked: {
                    takenOver = !takenOver
                    Controller.setTakeOverAndPlay(takenOver)
                }

                resetBtn.onClicked: {
                    setUp(itemsInRow)
                }
            }

            model: ListModel {
                id: colorModel
            }

            delegate: ColorDelagate{
                colorAnim.duration: speed * 4
                mouseArea.enabled: delagateEnabled
                mouseArea.onClicked: {
                    clickHappned(index)
                    //                    clickQueue.push(index);
                    //                    if (!clickTimer.running)
                    //                    clickTimer.start()
                }
            }

        }

    }


    function clickHappned(index) {

        var colorState = colorModel.get(index).colorState
        if (colorState === 0) {

            var currIndex = index - itemsInRow - 1;
            var startIndex = currIndex + 1;
            var flag = false;

            for (var i = 0; i < 3; i++) {
                for (var j = 0; j < 3; j++) {
                    if (currIndex >= 0 && currIndex < itemCount) {
                        if ((Math.floor((currIndex) / itemsInRow)) === (Math.floor((startIndex / itemsInRow)) + i)
                                || index === currIndex) {
                            var item = colorModel.get(currIndex);
                            if (item.colorState !== 0) {
                                buttonAnimQueue.push(item);
                                flag = true;
                            }
                        }
                    }
                    currIndex++;

                    if (j === 2) {
                        currIndex += itemsInRow - 3;
                    }
                }

            }
            if (flag) {
                buttonAnimQueue.push(0);
                buttonAnimQueue.push(colorModel.get(index));
                Controller.clicked(index)
            }


            delagateEnabled = false
            animationTimer.start()
        }
    }

    Rectangle {

        width: (root.width - gridView.width - 100) > 250 ? (root.width - gridView.width - 100) : 250
        height: root.height - 100
        color: "transparent"

        anchors.verticalCenter: gridContainer.verticalCenter
        anchors.left: gridContainer.right
        anchors.leftMargin: 15
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#66000000"
            radius: 5
        }

        Column {
            anchors.fill: parent
            spacing: 5

            Rectangle {
                width: parent.width
                height: parent.height / 2 - 10
                radius: 5
                color: redPlayerColor

                Text {
                    id:redText
                    color: "white"
                    anchors.centerIn: parent

                    font {
                        pointSize:  parent.width / 5
                    }

                }
            }

            Rectangle {
                width: parent.width
                height: parent.height / 2 - 10
                color: greenPlayerColor

                radius: 5
                Text {
                    id: greenText
                    color: "white"
                    anchors.centerIn: parent

                    font {
                        pointSize:  parent.width / 5
                    }

                }
            }
        }

        Component.onCompleted: {
        }
    }

    Timer {
        id: clickTimer
        interval: 750 ; running: false; repeat: true
        onTriggered: {
            if (delagateEnabled && clickQueue.length !== 0) {
                clickHappned(clickQueue.shift())
            }

            if (clickQueue.length === 0)   {
                stop()
            }
        }
    }


    Timer {
        id: animationTimer
        interval: 50 ; running: false; repeat: true
        onTriggered: {
            if (buttonAnimQueue.length !== 0) {
                var item = buttonAnimQueue.shift();
                if (item === 0) {
                    item = buttonAnimQueue.shift();
                    item.score = 1
                    item.m_color = String(Qt.darker(redTurn ? redPlayerColor : greenPlayerColor, getAmount(item.score)));
                    item.colorState = redTurn ? 1 : 2;
                    redTurn = !redTurn;
                    colorAnim.restart()
                } else {
                    item.score++
                    item.m_color = String(Qt.darker(redTurn ? redPlayerColor : greenPlayerColor, getAmount(item.score)));
                    item.colorState = redTurn ? 1 : 2;
                }
            } else {
                updateScores()
                delagateEnabled = true
                stop()

            }
        }
    }

    function getAmount(input) {
        return 1 + input / 8
    }

    function updateScores() {
        var redScore = 0
        var greenScore = 0;
        var flag = false;
        for (var i = 0; i < colorModel.count; i++) {
            var item = colorModel.get(i);
            if (item.colorState === 1) {
                redScore += item.score;
            } else if (item.colorState === 2) {
                greenScore += item.score;
            } else if (colorModel.get(i).colorState === 0) {
                flag = true;
            }
        }

        redText.text = redScore
        greenText.text = greenScore


        if (!flag) {
            takenOver = false;
            root.color = ((redScore > greenScore) ? "#ef9a9a" : "#80CBC4")
        }
    }

    Connections {
        target: Controller
        onSubmitClick: {
            clickQueue.push(item);
            if (!clickTimer.running)
                clickTimer.start()
        }
    }

}
