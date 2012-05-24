#include "vianalysiscontainer.h"
#include "ui_vianalysiscontainer.h"

ViAnalysisContainer::ViAnalysisContainer(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViAnalysisContainer();
	mUi->setupUi(this);
}

ViAnalysisContainer::~ViAnalysisContainer()
{
	delete mUi;
}
