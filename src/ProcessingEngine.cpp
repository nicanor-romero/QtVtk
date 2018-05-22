#include "ProcessingEngine.h"

#include <thread>
#include <memory>

#include <QDebug>
#include <QFileInfo>

#include <vtkAlgorithmOutput.h>
#include <vtkOBJReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include "Model.h"


ProcessingEngine::ProcessingEngine()
{
}


std::shared_ptr<Model> ProcessingEngine::addModel(QUrl modelFilePath)
{
	qDebug() << "ProcessingEngine::addModelData()";

	QString modelFilePathExtension = QFileInfo(modelFilePath.toString()).suffix().toLower();

    vtkSmartPointer<vtkOBJReader> objReader = vtkSmartPointer<vtkOBJReader>::New();
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkPolyData> inputData;

    if (modelFilePathExtension == ".obj")
    {
        // Read OBJ file
		objReader->SetFileName(modelFilePath.toString().toStdString().c_str());
        objReader->Update();
        inputData = objReader->GetOutput();
    }
    else
    {
        // Read STL file
		stlReader->SetFileName(modelFilePath.toString().toStdString().c_str());
        stlReader->Update();
        inputData = stlReader->GetOutput();
    }

	// Preprocess the polydata
	vtkSmartPointer<vtkPolyData> preprocessedPolydata = preprocessPolydata(inputData);

	// Create Model instance and insert it into the vector
	std::shared_ptr<Model> model = std::make_shared<Model>(preprocessedPolydata);
	m_models.push_back(model);

	return m_models[m_models.size() - 1];
}


vtkSmartPointer<vtkPolyData> ProcessingEngine::preprocessPolydata(vtkSmartPointer<vtkPolyData> inputData)
{
	// Center the polygon
	double center[3];
	inputData->GetCenter(center);

	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->Translate(-center[0], -center[1], -center[2]);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(inputData);
	transformFilter->SetTransform(translation);
	transformFilter->Update();

	// Normals - For the Gouraud interpolation to work
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData(transformFilter->GetOutput());
	normals->ComputePointNormalsOn();
	normals->Update();

	return normals->GetOutput();
}

void ProcessingEngine::placeModel(std::shared_ptr<Model> model)
{
	qDebug() << "ProcessingEngine::placeModel()";

	model->translateToPosition(0, 0);
}

std::vector<std::shared_ptr<Model> > ProcessingEngine::getModels()
{
	return m_models;
}
