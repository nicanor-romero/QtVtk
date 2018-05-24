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

    Material.primary: Material.Indigo
    Material.accent: Material.LightBlue

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

            ToolTip.visible: hovered
            ToolTip.delay: 1000
            ToolTip.text: "Open a 3D model into the canvas"
        }

        ComboBox {
            id: representationCombobox
            visible: canvasHandler.isModelSelected
            width: 200
            model: ["Points", "Wireframe", "Surface"]
            currentIndex: 2
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 50

            onActivated: canvasHandler.setModelsRepresentation(currentIndex);
        }

        Slider {
            id: opacitySlider
            visible: canvasHandler.isModelSelected
            width: 200
            value: 1
            from: 0.1
            to: 1
            stepSize: 0.01
            anchors.left: parent.left
            anchors.top: representationCombobox.bottom
            anchors.margins: 50

            onValueChanged: canvasHandler.setModelsOpacity(value);
        }

        Label {
            id: positionLabelX
            visible: canvasHandler.isModelSelected
            text: "X: " + canvasHandler.modelPositionX
            font.pixelSize: 12
            anchors.bottom: positionLabelY.top
            anchors.left: parent.left
            anchors.margins: 20
        }

        Label {
            id: positionLabelY
            visible: canvasHandler.isModelSelected
            text: "Y: " + canvasHandler.modelPositionY
            font.pixelSize: 12
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 20
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
