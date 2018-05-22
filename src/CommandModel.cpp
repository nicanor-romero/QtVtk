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

bool CommandModel::addToStack()
{
	return true;
}

void CommandModel::redo()
{
}

void CommandModel::undo()
{
}

void CommandModel::execute()
{
	this->redo();
}
