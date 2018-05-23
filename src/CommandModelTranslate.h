#ifndef COMMANDMODELTRANSLATE_H
#define COMMANDMODELTRANSLATE_H

#include <memory>

#include <QVector>

#include "CommandModel.h"


class Model;
class QVTKFramebufferObjectRenderer;

class CommandModelTranslate : public CommandModel
{
public:
	typedef struct
	{
		std::shared_ptr<Model> model{nullptr};
		double previousPositionX{0};
		double previousPositionY{0};
		int screenX{0};
		int screenY{0};
		double targetPositionX{0};
		double targetPositionY{0};
	} TranslateParams_t;

	CommandModelTranslate(QVTKFramebufferObjectRenderer *vtkFboRenderer, const TranslateParams_t & translateVector, bool inTransition);

	void execute() override;

private:
	void transformCoordinates();

	TranslateParams_t m_translateParams;
	bool m_inTransition;
	bool m_needsTransformation = true;
};

#endif // COMMANDMODELTRANSLATE_H
