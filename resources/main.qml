import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.3
import QtQuick.Controls.Material 2.2
import QtVTK 1.0


ApplicationWindow {
    id: root
    minimumWidth: 1024
    minimumHeight: 700
    visible: true
    title: "QtVTK"

    Material.accent: Material.Blue

    Rectangle {
        id: screenCanvasUI
        anchors.fill: parent

        VtkFboItem {
            id: vtkFboItem
            objectName: "vtkFboItem"
            anchors.fill: parent

            MouseArea {
                acceptedButtons: Qt.LeftButton
                anchors.fill: parent

                onPositionChanged: {
                    canvasHandler.mouseMoveEvent(pressedButtons, mouseX, mouseY);
                }
                onPressed: {
                    canvasHandler.mousePressEvent(pressedButtons, mouseX, mouseY);
                }
                onReleased: {
                    canvasHandler.mouseReleaseEvent(pressedButtons, mouseX, mouseY);
                }
            }
        }

        Button {
            id: openFileButton
            text: "Open file"
            highlighted: true
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 50
            onClicked: canvasHandler.showFileDialog = true;
        }

        Label {
            id: positionLabelX
            text: "X: " + canvasHandler.modelPositionX
            color: "white"
            font.pixelSize: 20
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 50
        }

        Label {
            id: positionLabelY
            text: "Y: " + canvasHandler.modelPositionY
            color: "white"
            font.pixelSize: 20
            anchors.top: positionLabelX.bottom
            anchors.left: parent.left
            anchors.margins: 50
        }
    }

    FileDialog {
        id: openModelsFileDialog
        visible: canvasHandler.showFileDialog
        title: "Import model"
        folder: shortcuts.documents
        nameFilters: ["Model files" + "(*.stl *.STL *.obj *.OBJ)", "All files" + "(*)"]

        onAccepted: {
            canvasHandler.showFileDialog = false;
            canvasHandler.openModel(fileUrl);
        }
        onRejected: {
            canvasHandler.showFileDialog = false;
        }
    }
}
