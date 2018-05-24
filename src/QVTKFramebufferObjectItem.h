#ifndef QVTKFRAMEBUFFEROBJECTITEM_H
#define QVTKFRAMEBUFFEROBJECTITEM_H

#include <memory>
#include <queue>
#include <mutex>

#include <QtQuick/QQuickFramebufferObject>

#include "CommandModelTranslate.h"


class CommandModel;
class Model;
class ProcessingEngine;
class QVTKFramebufferObjectRenderer;

class QVTKFramebufferObjectItem : public QQuickFramebufferObject
{
	Q_OBJECT

public:
	QVTKFramebufferObjectItem();

	Renderer *createRenderer() const Q_DECL_OVERRIDE;
	void setVtkFboRenderer(QVTKFramebufferObjectRenderer*);
	bool isInitialized();
	void setProcessingEngine(std::shared_ptr<ProcessingEngine> processingEngine);

	// Model releated functions
	bool isModelSelected();

	double getSelectedModelPositionX();
	double getSelectedModelPositionY();

	void selectModel(int screenX, int screenY);
	void resetModelSelection();
	void addModelFromFile(QUrl modelPath);

	void translateModel(CommandModelTranslate::TranslateParams_t &translateData, bool inTransition);

	std::queue<CommandModel*> commandsQueue;
	std::mutex commandsQueueMutex;

	// Camera related functions
	void wheelEvent(QWheelEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;

	QMouseEvent *getLastMouseLeftButton();
	QMouseEvent *getLastMouseButton();
	QMouseEvent *getLastMoveEvent();
	QWheelEvent *getLastWheelEvent();

	void resetCamera();

	int getModelsRepresentation();
	double getModelsOpacity();
	bool getGourauInterpolation();
	int getModelColorR();
	int getModelColorG();
	int getModelColorB();

	void setModelsRepresentation(int representationOption);
	void setModelsOpacity(double opacity);
	void setGouraudInterpolation(bool gouraudInterpolation);
	void setModelColorR(int colorR);
	void setModelColorG(int colorG);
	void setModelColorB(int colorB);

signals:
	void rendererInitialized();

	void isModelSelectedChanged();
	void selectedModelPositionXChanged();
	void selectedModelPositionYChanged();

	void addModelFromFileDone();
	void addModelFromFileError(QString error);

private:
	void addCommand(CommandModel* command);

	QVTKFramebufferObjectRenderer *m_vtkFboRenderer = nullptr;
	std::shared_ptr<ProcessingEngine> m_processingEngine;

	QMouseEvent *m_lastMouseLeftButton;
	QMouseEvent *m_lastMouseButton;
	QMouseEvent *m_lastMouseMove;
	QWheelEvent *m_lastMouseWheel;

	int m_modelsRepresentationOption = 2;
	double m_modelsOpacity = 1.0;
	bool m_gouraudInterpolation = false;
	int m_modelColorR = 3;
	int m_modelColorG = 169;
	int m_modelColorB = 244;
};

#endif // QVTKFRAMEBUFFEROBJECTITEM_H
