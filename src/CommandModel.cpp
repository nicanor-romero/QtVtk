#include "CommandModel.h"
#include "QVTKFramebufferObjectRenderer.h"


CommandModel::CommandModel(QVTKFramebufferObjectRenderer *vtkFboRenderer)
	: m_vtkFboRenderer{vtkFboRenderer}
{
}


bool CommandModel::isReady()
{
	return true;
}

void CommandModel::execute()
{
}
