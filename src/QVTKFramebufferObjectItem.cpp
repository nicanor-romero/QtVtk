#include "CommandModel.h"
#include "CommandModelAdd.h"
#include "Model.h"
#include "ProcessingEngine.h"
#include "QVTKFramebufferObjectItem.h"
#include "QVTKFramebufferObjectRenderer.h"


QVTKFramebufferObjectItem::QVTKFramebufferObjectItem()
{
	m_lastMouseLeftButton = new QMouseEvent(QEvent::None, QPointF(0,0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	m_lastMouseButton = new QMouseEvent(QEvent::None, QPointF(0,0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	m_lastMouseMove = new QMouseEvent(QEvent::None, QPointF(0,0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	m_lastMouseWheel = new QWheelEvent(QPointF(0,0), 0, Qt::NoButton, Qt::NoModifier, Qt::Vertical);

	this->setMirrorVertically(true); // QtQuick and OpenGL have opposite Y-Axis directions

	setAcceptedMouseButtons(Qt::RightButton);
}


QQuickFramebufferObject::Renderer *QVTKFramebufferObjectItem::createRenderer() const
{
	qDebug() << "QVTKFramebufferObjectItem::createRenderer";
	return new QVTKFramebufferObjectRenderer();
}

void QVTKFramebufferObjectItem::setVtkFboRenderer(QVTKFramebufferObjectRenderer* renderer)
{
	qDebug() << "QVTKFramebufferObjectItem::setVtkFboRenderer";

	m_vtkFboRenderer = renderer;

	connect(m_vtkFboRenderer, &QVTKFramebufferObjectRenderer::selectedModelPositionXChanged, this, &QVTKFramebufferObjectItem::selectedModelPositionXChanged);
	connect(m_vtkFboRenderer, &QVTKFramebufferObjectRenderer::selectedModelPositionYChanged, this, &QVTKFramebufferObjectItem::selectedModelPositionYChanged);

	m_vtkFboRenderer->setProcessingEngine(m_processingEngine);
}

bool QVTKFramebufferObjectItem::isInitialized()
{
	return (m_vtkFboRenderer != nullptr);
}

void QVTKFramebufferObjectItem::setProcessingEngine(std::shared_ptr<ProcessingEngine> processingEngine)
{
	m_processingEngine = std::shared_ptr<ProcessingEngine>(processingEngine);
}


// Model releated functions

bool QVTKFramebufferObjectItem::isModelSelected()
{
	if (m_vtkFboRenderer->selectedModelMutex.try_lock())
	{
		m_vtkFboRenderer->selectedModelMutex.unlock();
		return m_vtkFboRenderer->isModelSelected();
	}
	return false;
}

double QVTKFramebufferObjectItem::getSelectedModelPositionX()
{
	return m_vtkFboRenderer->getSelectedModelPositionX();
}

double QVTKFramebufferObjectItem::getSelectedModelPositionY()
{
	return m_vtkFboRenderer->getSelectedModelPositionY();
}


void QVTKFramebufferObjectItem::selectModel(int screenX, int screenY)
{
	if (m_vtkFboRenderer->selectedModelMutex.try_lock())
	{
		*m_lastMouseLeftButton = QMouseEvent(QEvent::None, QPointF(screenX, screenY), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		m_lastMouseLeftButton->ignore();

		update();
	}
}

void QVTKFramebufferObjectItem::resetModelSelection()
{
	if (m_vtkFboRenderer->selectedModelMutex.try_lock())
	{
		*m_lastMouseLeftButton = QMouseEvent(QEvent::None, QPointF(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		m_lastMouseLeftButton->ignore();

		update();
	}
}

void QVTKFramebufferObjectItem::addModelFromFile(QUrl modelPath)
{
	qDebug() << "QVTKFramebufferObjectItem::addModelFromFile";

	CommandModelAdd *command = new CommandModelAdd(m_vtkFboRenderer, m_processingEngine, modelPath);

	connect(command, &CommandModelAdd::ready, this, &QVTKFramebufferObjectItem::update);
	connect(command, &CommandModelAdd::done, this, &QVTKFramebufferObjectItem::addModelFromFileDone);

	command->start();

	this->addCommand(command);
}

void QVTKFramebufferObjectItem::translateModel(CommandModelTranslate::TranslateParams_t & translateData, bool inTransition)
{
	if (translateData.model == nullptr)
	{
		// If no model selected yet, try to select one
		translateData.model = m_vtkFboRenderer->getSelectedModel();

		if (translateData.model == nullptr)
		{
			return;
		}
	}

	this->addCommand(new CommandModelTranslate(m_vtkFboRenderer, translateData, inTransition));
}


void QVTKFramebufferObjectItem::addCommand(CommandModel *command)
{
	commandsQueueMutex.lock();
	commandsQueue.push(command);
	commandsQueueMutex.unlock();

	update();
}


// Camera related functions

void QVTKFramebufferObjectItem::wheelEvent(QWheelEvent *e)
{
	*m_lastMouseWheel = *e;
	m_lastMouseWheel->ignore();
	e->accept();
	update();
}

void QVTKFramebufferObjectItem::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::RightButton)
	{
		*m_lastMouseButton = *e;
		m_lastMouseButton->ignore();
		e->accept();
		update();
	}
}

void QVTKFramebufferObjectItem::mouseReleaseEvent(QMouseEvent *e)
{
	*m_lastMouseButton = *e;
	m_lastMouseButton->ignore();
	e->accept();
	update();
}

void QVTKFramebufferObjectItem::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::RightButton)
	{
		*m_lastMouseMove = *e;
		m_lastMouseMove->ignore();
		e->accept();
		update();
	}
}


QMouseEvent *QVTKFramebufferObjectItem::getLastMouseLeftButton()
{
	return m_lastMouseLeftButton;
}

QMouseEvent *QVTKFramebufferObjectItem::getLastMouseButton()
{
	return m_lastMouseButton;
}

QMouseEvent *QVTKFramebufferObjectItem::getLastMoveEvent()
{
	return m_lastMouseMove;
}

QWheelEvent *QVTKFramebufferObjectItem::getLastWheelEvent()
{
	return m_lastMouseWheel;
}


void QVTKFramebufferObjectItem::resetCamera()
{
	m_vtkFboRenderer->resetCamera();
	update();
}
