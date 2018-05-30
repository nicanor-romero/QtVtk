#ifndef PROCESSINGENGINE_H
#define PROCESSINGENGINE_H

#include <array>
#include <cstdint>
#include <vector>
#include <mutex>
#include <memory>

#include <QUrl>

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>


class Model;

class ProcessingEngine
{
	public:
		ProcessingEngine();

		const std::shared_ptr<Model>& addModel(const QUrl &modelFilePath);

		void placeModel(Model &model) const;

		std::vector<std::shared_ptr<Model>> getModels() const;

	private:
		vtkSmartPointer<vtkPolyData> preprocessPolydata(const vtkSmartPointer<vtkPolyData> inputData) const;

		std::vector<std::shared_ptr<Model>> m_models;
};

#endif // PROCESSINGENGINE_H
