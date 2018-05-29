#ifndef QVTKFRAMEBUFFEROBJECTRENDERER_H
#define QVTKFRAMEBUFFEROBJECTRENDERER_H

#include <memory>
#include <vector>
#include <mutex>

#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>
#include <QUndoStack>
#include <QDir>

#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkCubeSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkObject.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>


class Model;
class QVTKFramebufferObjectItem;
class ProcessingEngine;

class QVTKFramebufferObjectRenderer : public QObject, public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	QVTKFramebufferObjectRenderer();

	virtual void synchronize(QQuickFramebufferObject *item);
	virtual void render();
	virtual void openGLInitState();
	QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

	void addModelActor(std::shared_ptr<Model> model);

	std::mutex selectedModelMutex;
	std::shared_ptr<Model> getSelectedModel();
	bool isModelSelected();

	void resetCamera();
	bool screenToWorld(int16_t screenX, int16_t screenY, double worldPos[3]);

	void setSelectedModelPositionX(double positionX);
	void setSelectedModelPositionY(double positionY);

	double getSelectedModelPositionX();
	double getSelectedModelPositionY();

	void setProcessingEngine(std::shared_ptr<ProcessingEngine> processingEngine);

signals:
	void isModelSelectedChanged();

	void selectedModelPositionXChanged();
	void selectedModelPositionYChanged();

private:
	void initScene();
	void generatePlatform();
	void updatePlatform();

	void selectModel(int16_t x, int16_t y);
	void clearSelectedModel();
	void setIsModelSelected(bool isModelSelected);

	void createLine(double x1, double y1, double z1, double x2, double y2, double z2, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkCellArray> cells);
	std::shared_ptr<Model> getSelectedModelNoLock();

	std::shared_ptr<ProcessingEngine> m_processingEngine;
	QVTKFramebufferObjectItem *m_vtkFboItem = nullptr;
	vtkSmartPointer<vtkExternalOpenGLRenderWindow> m_vtkRenderWindow;
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkGenericRenderWindowInteractor> m_vtkRenderWindowInteractor;

	vtkSmartPointer<vtkCellPicker> m_picker;

	std::shared_ptr<Model> m_selectedModel = nullptr;
	vtkSmartPointer<vtkActor> m_selectedActor = nullptr;
	bool m_isModelSelected = false;

	double m_selectedModelPositionX = 0.0;
	double m_selectedModelPositionY = 0.0;

	QMouseEvent *m_mouseLeftButton;
	QMouseEvent *m_mouseEvent;
	QMouseEvent *m_moveEvent;
	QWheelEvent *m_wheelEvent;

	vtkSmartPointer<vtkCubeSource> m_platformModel;
	vtkSmartPointer<vtkPolyData> m_platformGrid;
	vtkSmartPointer<vtkActor> m_platformModelActor;
	vtkSmartPointer<vtkActor> m_platformGridActor;

	double m_platformWidth = 200.0;
	double m_platformDepth = 200.0;
	double m_platformHeight = 200.0;
	double m_platformThickness = 2.0;
	double m_gridBottomHeight = 0.15;
	uint16_t m_gridSize = 10;

	double m_camPositionX;
	double m_camPositionY;
	double m_camPositionZ;

	double m_clickPositionZ = 0.0;

	bool m_firstRender = true;

	int m_modelsRepresentationOption = 0;
	double m_modelsOpacity = 1.0;
	bool m_gouraudInterpolation = false;
};

#endif // QVTKFRAMEBUFFEROBJECTRENDERER_H
