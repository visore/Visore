#include "viprojectcorrectionwidget.h"
#include "ui_viprojectcorrectionwidget.h"

ViProjectCorrectionWidget::ViProjectCorrectionWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectCorrectionWidget();
	mUi->setupUi(this);

	//Font
	QFont font;
	font.setFamily("Harabara");
	font.setPointSize(16);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	QColor color = ViThemeManager::color(ViThemeColors::TextColor1);

	//Button1
	QObject::connect(mUi->button1, SIGNAL(clicked()), this, SLOT(correct()));
	mUi->button1->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->button1->setText("Process", color, font);
	mUi->button1->setSize(140, 60);

	//Button2
	QObject::connect(mUi->button2, SIGNAL(clicked()), this, SLOT(correct()));
	mUi->button2->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->button2->setText("Process", color, font);
	mUi->button2->setSize(140, 60);
}

ViProjectCorrectionWidget::~ViProjectCorrectionWidget()
{
	delete mUi;
}

void ViProjectCorrectionWidget::correct()
{
	engine()->correct(mUi->projectLoader->objects(), mUi->neuralSelector->corrector());
}
