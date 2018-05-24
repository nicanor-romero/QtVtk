#ifndef CANVASHANDLER_H
#define CANVASHANDLER_H

#include <memory>

#include <QObject>
#include <QUrl>


class ProcessingEngine;
class QVTKFramebufferObjectItem;

class CanvasHandler : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool showFileDialog MEMBER m_showFileDialog NOTIFY showFileDialogChanged)
	Q_PROPERTY(bool isModelSelected READ getIsModelSelected NOTIFY isModelSelectedChanged)
	Q_PROPERTY(double modelPositionX READ getSelectedModelPositionX NOTIFY selectedModelPositionXChanged)
	Q_PROPERTY(double modelPositionY READ getSelectedModelPositionY NOTIFY selectedModelPositionYChanged)

public:
	CanvasHandler(int argc, char **argv);

	Q_INVOKABLE void openModel(QUrl path);

	Q_INVOKABLE void mousePressEvent(int button, int mouseX, int mouseY);
	Q_INVOKABLE void mouseMoveEvent(int button, int mouseX, int mouseY);
	Q_INVOKABLE void mouseReleaseEvent(int button, int mouseX, int mouseY);

	bool getIsModelSelected();
	double getSelectedModelPositionX();
	double getSelectedModelPositionY();

	Q_INVOKABLE void setModelsRepresentation(int representationOption);
	Q_INVOKABLE void setModelsOpacity(double opacity);
	Q_INVOKABLE void setGouraudInterpolation(bool gouraudInterpolation);
	Q_INVOKABLE void setModelColorR(int colorR);
	Q_INVOKABLE void setModelColorG(int colorG);
	Q_INVOKABLE void setModelColorB(int colorB);

public slots:
	void startApplication();

signals:
	void showFileDialogChanged();

	void isModelSelectedChanged();
	void selectedModelPositionXChanged();
	void selectedModelPositionYChanged();

private:
	void closeApplication();
	bool isModelExtensionValid(QUrl modelPath);

	std::shared_ptr<ProcessingEngine> m_processingEngine;
	QVTKFramebufferObjectItem *m_vtkFboItem = nullptr;

	double m_previousWorldX = 0;
	double m_previousWorldY = 0;
	bool m_draggingMouse = false;
	bool m_showFileDialog = false;
};

#endif // CANVASHANDLER_H
