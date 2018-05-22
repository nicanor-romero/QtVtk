#include <QDebug>

#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "Model.h"


QColor Model::m_defaultModelColor = QColor{"#0277bd"};
QColor Model::m_selectedModelColor = QColor{"#004c8c"};


Model::Model(vtkSmartPointer<vtkPolyData> modelData)
	: m_modelData{modelData}
{
	// Place model with lower Z bound at zero
	m_positionZ = -m_modelData->GetBounds()[4];

	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->Translate(m_positionX, m_positionY, m_positionZ);

	m_modelFilterTranslate = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	m_modelFilterTranslate->SetInputData(m_modelData);
	m_modelFilterTranslate->SetTransform(translation);
	m_modelFilterTranslate->Update();

	// Model Mapper
	m_modelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_modelMapper->SetInputConnection(m_modelFilterTranslate->GetOutputPort());
	m_modelMapper->ScalarVisibilityOff();

	// Model Actor
	m_modelActor = vtkSmartPointer<vtkActor>::New();
	m_modelActor->SetMapper(m_modelMapper);
	m_modelActor->GetProperty()->SetInterpolationToFlat();

	m_modelActor->GetProperty()->SetAmbient(0.1);
	m_modelActor->GetProperty()->SetDiffuse(0.7);
	m_modelActor->GetProperty()->SetSpecular(0.3);
	this->setColor(m_defaultModelColor);

	m_modelActor->SetPosition(0.0, 0.0, 0.0);
}


double Model::getPositionX()
{
	m_propertiesMutex.lock();
	double positionX = m_positionX;
	m_propertiesMutex.unlock();
	return positionX;
}

double Model::getPositionY()
{
	m_propertiesMutex.lock();
	double positionY = m_positionY;
	m_propertiesMutex.unlock();
	return positionY;
}


void Model::translateToPosition(double x, double y)
{
	if (m_positionX == x && m_positionY == y)
	{
		return;
	}

	m_propertiesMutex.lock();
	m_positionX = x;
	m_positionY = y;
	m_propertiesMutex.unlock();

	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->Translate(m_positionX, m_positionY, m_positionZ);
	m_modelFilterTranslate->SetTransform(translation);
	m_modelFilterTranslate->Update();
}


void Model::setPositionXChanged()
{
	emit positionXChanged(m_positionX);
}

void Model::setPositionYChanged()
{
	emit positionYChanged(m_positionY);
}


vtkSmartPointer<vtkActor> Model::getModelActor()
{
	return m_modelActor;
}


void Model::setSelected(bool selected)
{
	if (m_selected != selected)
	{
		m_selected = selected;

		this->updateModelColor();
	}
}

void Model::updateModelColor()
{
	if (m_selected)
	{
		this->setColor(m_selectedModelColor);
	}
	else
	{
		this->setColor(m_defaultModelColor);
	}
}

void Model::setColor(QColor color)
{
	m_modelActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
}

double Model::getMouseDeltaX()
{
	return m_mouseDeltaX;
}

double Model::getMouseDeltaY()
{
	return m_mouseDeltaY;
}

void Model::setMouseDeltaXY(double deltaX, double deltaY)
{
	m_mouseDeltaX = deltaX;
	m_mouseDeltaY = deltaY;
}
