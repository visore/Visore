#include "viprojectcorrectionwidget.h"
#include "ui_viprojectcorrectionwidget.h"

ViProjectCorrectionWidget::ViProjectCorrectionWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectCorrectionWidget();
	mUi->setupUi(this);

	QObject::connect(mUi->projectLoader, SIGNAL(started()), this, SLOT(correct()));
}

ViProjectCorrectionWidget::~ViProjectCorrectionWidget()
{
	delete mUi;
}

void ViProjectCorrectionWidget::correct()
{
	ViProject *project = mUi->projectLoader->project();
	if(project != NULL)
	{
		engine()->correct(*project);
	}
}
