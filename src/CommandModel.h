#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <QUndoCommand>


class QVTKFramebufferObjectRenderer;

class CommandModel : public QUndoCommand
{
public:
	virtual bool isReady();
	virtual bool addToStack();
	virtual void undo();
	virtual void redo();
	virtual void execute();

protected:
	CommandModel(QVTKFramebufferObjectRenderer *vtkFboRenderer);

	QVTKFramebufferObjectRenderer *m_vtkFboRenderer;
};

#endif // COMMANDMODEL_H
