#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <mutex>

#include <QObject>
#include <QColor>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>


class Model : public QObject
{
	Q_OBJECT

public:
	Model(vtkSmartPointer<vtkPolyData> modelData);

	vtkSmartPointer<vtkActor> getModelActor();
	double getMouseDeltaX();
	double getMouseDeltaY();

	double getPositionX();
	double getPositionY();

	void translate(double deltaX, double deltaY);
	void translateToPosition(double x, double y);

	void setSelected(bool selected);
	void setMouseDeltaXY(double deltaX, double deltaY);

	void updateModelColor();
	void setColor(QColor color);

signals:
	void positionXChanged(double positionX);
	void positionYChanged(double positionY);

private:
	static QColor m_defaultModelColor;
	static QColor m_selectedModelColor;

	void setPositionXChanged();
	void setPositionYChanged();

	vtkSmartPointer<vtkPolyData> m_modelData;
	vtkSmartPointer<vtkPolyDataMapper> m_modelMapper;
	vtkSmartPointer<vtkActor> m_modelActor;

	vtkSmartPointer<vtkTransformPolyDataFilter> m_modelFilterTranslate;

	std::mutex m_propertiesMutex;

	double m_positionX {0.0};
	double m_positionY {0.0};
	double m_positionZ {0.0};

	bool m_selected = false;

	double m_mouseDeltaX = 0.0;
	double m_mouseDeltaY = 0.0;
};


#endif // MODEL_H
