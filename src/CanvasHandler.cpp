#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "Model.h"
#include "ProcessingEngine.h"
#include "QVTKFramebufferObjectItem.h"
#include "QVTKFramebufferObjectRenderer.h"
#include "CanvasHandler.h"


CanvasHandler::CanvasHandler(int argc, char **argv)
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;

	app.setApplicationName("QtVTK");
	app.setWindowIcon(QIcon(":/resources/bq.ico"));

	// Register QML types
	qmlRegisterType<QVTKFramebufferObjectItem>("QtVTK", 1, 0, "VtkFboItem");

	// Create classes instances
	m_processingEngine = std::shared_ptr<ProcessingEngine>(new ProcessingEngine());

	// Expose C++ classes to QML
	QQmlContext* ctxt = engine.rootContext();

	ctxt->setContextProperty("canvasHandler", this);

	QQuickStyle::setStyle("Material");

	// Load main QML file
	engine.load(QUrl("qrc:/resources/main.qml"));

	// Get reference to the QVTKFramebufferObjectItem created in QML
	// We cannot use smart pointers because this object must be deleted by QML
	QObject *rootObject = engine.rootObjects().first();
	m_vtkFboItem = rootObject->findChild<QVTKFramebufferObjectItem*>("vtkFboItem");

	// Give the vtkFboItem reference to the CanvasHandler
	if (m_vtkFboItem)
	{
		qDebug() << "CanvasHandler::CanvasHandler: setting vtkFboItem to CanvasHandler";

		m_vtkFboItem->setProcessingEngine(m_processingEngine);

		connect(m_vtkFboItem, &QVTKFramebufferObjectItem::rendererInitialized, this, &CanvasHandler::startApplication);
		connect(m_vtkFboItem, &QVTKFramebufferObjectItem::isModelSelectedChanged, this, &CanvasHandler::isModelSelectedChanged);
		connect(m_vtkFboItem, &QVTKFramebufferObjectItem::selectedModelPositionXChanged, this, &CanvasHandler::selectedModelPositionXChanged);
		connect(m_vtkFboItem, &QVTKFramebufferObjectItem::selectedModelPositionYChanged, this, &CanvasHandler::selectedModelPositionYChanged);
	}
	else
	{
		qCritical() << "CanvasHandler::CanvasHandler: Unable to get vtkFboItem instance";
		return;
	}

	int rc = app.exec();

	qDebug() << "CanvasHandler::CanvasHandler: Execution finished with return code:" << rc;
}


void CanvasHandler::startApplication()
{
	qDebug() << "CanvasHandler::startApplication()";

	disconnect(m_vtkFboItem, &QVTKFramebufferObjectItem::rendererInitialized, this, &CanvasHandler::startApplication);
}

void CanvasHandler::closeApplication()
{
	QApplication::instance()->quit();
}


void CanvasHandler::openModel(QUrl path)
{
	qDebug() << "CanvasHandler::openModel():" << path;

	if (path.isLocalFile())
	{
		// Remove the "file:///" if present
		path = path.toLocalFile();
	}

	m_vtkFboItem->addModelFromFile(path);
}

bool CanvasHandler::isModelExtensionValid(QUrl modelPath)
{
	if (modelPath.toString().toLower().endsWith(".stl") || modelPath.toString().toLower().endsWith(".obj"))
	{
		return true;
	}

	return false;
}


void CanvasHandler::mousePressEvent(int button, int screenX, int screenY)
{
	qDebug() << "CanvasHandler::mousePressEvent()";

	m_vtkFboItem->selectModel(screenX, screenY);
}

void CanvasHandler::mouseMoveEvent(int button, int screenX, int screenY)
{
	if (!m_vtkFboItem->isModelSelected())
	{
		return;
	}

	if (!m_draggingMouse)
	{
		m_draggingMouse = true;

		m_previousWorldX = m_vtkFboItem->getSelectedModelPositionX();
		m_previousWorldY = m_vtkFboItem->getSelectedModelPositionY();
	}

	CommandModelTranslate::TranslateParams_t translateParams;

	translateParams.screenX = screenX;
	translateParams.screenY = screenY;

	m_vtkFboItem->translateModel(translateParams, true);
}

void CanvasHandler::mouseReleaseEvent(int button, int screenX, int screenY)
{
	qDebug() << "CanvasHandler::mouseReleaseEvent()";

	if (!m_vtkFboItem->isModelSelected())
	{
		return;
	}

	if (m_draggingMouse)
	{
		m_draggingMouse = false;

		CommandModelTranslate::TranslateParams_t translateParams;

		translateParams.screenX = screenX;
		translateParams.screenY = screenY;
		translateParams.previousPositionX = m_previousWorldX;
		translateParams.previousPositionY = m_previousWorldY;

		m_vtkFboItem->translateModel(translateParams, false);
	}
}


bool CanvasHandler::getIsModelSelected()
{
	// QVTKFramebufferObjectItem might not be initialized when QML loads
	if (!m_vtkFboItem)
	{
		return 0;
	}

	return m_vtkFboItem->isModelSelected();
}

double CanvasHandler::getSelectedModelPositionX()
{
	// QVTKFramebufferObjectItem might not be initialized when QML loads
	if (!m_vtkFboItem)
	{
		return 0;
	}

	return m_vtkFboItem->getSelectedModelPositionX();
}

double CanvasHandler::getSelectedModelPositionY()
{
	// QVTKFramebufferObjectItem might not be initialized when QML loads
	if (!m_vtkFboItem)
	{
		return 0;
	}

	return m_vtkFboItem->getSelectedModelPositionY();
}

void CanvasHandler::setModelsRepresentation(int representationOption)
{
	m_vtkFboItem->setModelsRepresentation(representationOption);
}

void CanvasHandler::setModelsOpacity(double opacity)
{
	m_vtkFboItem->setModelsOpacity(opacity);
}

void CanvasHandler::setGouraudInterpolation(bool gouraudInterpolation)
{
	m_vtkFboItem->setGouraudInterpolation(gouraudInterpolation);
}

void CanvasHandler::setModelColorR(int colorR)
{
	m_vtkFboItem->setModelColorR(colorR);
}

void CanvasHandler::setModelColorG(int colorG)
{
	m_vtkFboItem->setModelColorG(colorG);
}

void CanvasHandler::setModelColorB(int colorB)
{
	m_vtkFboItem->setModelColorB(colorB);
}
