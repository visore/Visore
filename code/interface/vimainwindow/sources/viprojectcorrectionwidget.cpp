#include "viprojectcorrectionwidget.h"
#include "ui_viprojectcorrectionwidget.h"
#include <vistackedwidget.h>
#include <vicorrelationwidget.h>

ViProjectCorrectionWidget::ViProjectCorrectionWidget(QWidget *parent)
    : ViWidget(parent)
{
    mUi = new Ui::ViProjectCorrectionWidget();
    mUi->setupUi(this);
    mUi->projectLoader->disableBufferSelection();

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
    QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
	//engine()->correct(mUi->projectLoader->objects(), mUi->neuralSelector->corrector());
}

void ViProjectCorrectionWidget::showCorrelation()
{
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
    ViCorrelationWidget *widget = new ViCorrelationWidget();
    widget->setProject(mUi->projectLoader->project());
	ViStackedWidget::showTemporaryWidget(widget);
}

void ViProjectCorrectionWidget::hideEvent(QHideEvent *event)
{
    //TODO: clear the project. Currently segfault since the correlation widget is still using it
    //mUi->projectLoader->clear();
}
