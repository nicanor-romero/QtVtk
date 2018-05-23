#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H


class QVTKFramebufferObjectRenderer;

class CommandModel
{
public:
	virtual bool isReady();
	virtual void execute();

protected:
	CommandModel(QVTKFramebufferObjectRenderer *vtkFboRenderer);

	QVTKFramebufferObjectRenderer *m_vtkFboRenderer;
};

#endif // COMMANDMODEL_H
