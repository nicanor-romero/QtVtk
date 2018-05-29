#ifndef PROCESSINGENGINE_H
#define PROCESSINGENGINE_H

#include <array>
#include <cstdint>
#include <vector>
#include <mutex>
#include <memory>

#include <QUrl>

#include <vtkSmartPointer.h>


class vtkPolyData;
class Model;

class ProcessingEngine
{
	public:
		ProcessingEngine();

		std::shared_ptr<Model> addModel(QUrl modelFilePath);

		void placeModel(std::shared_ptr<Model> model);

		std::vector<std::shared_ptr<Model>> getModels();

	private:
		vtkSmartPointer<vtkPolyData> preprocessPolydata(vtkSmartPointer<vtkPolyData> inputData);

		std::vector<std::shared_ptr<Model>> m_models;
};

#endif // PROCESSINGENGINE_H
