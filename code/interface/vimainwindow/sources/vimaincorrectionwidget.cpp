#include <vimaincorrectionwidget.h>
#include <ui_vimaincorrectionwidget.h>
#include <vistackedwidget.h>
#include <vimaincorrelationwidget.h>

ViMainCorrectionWidget::ViMainCorrectionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViMainCorrectionWidget();
    mUi->setupUi(this);

	mUi->projectLoader->setTypeMode(ViProjectLoader::NoTypes);
	QObject::connect(mUi->projectLoader, SIGNAL(finished()), this, SLOT(showCorrector()));
	QObject::connect(mUi->projectLoader, SIGNAL(trackChanged()), this, SLOT(hideCorrector()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(hideCorrector()));
	hideCorrector();

    //Font
    QFont font;
    font.setFamily("Harabara");
    font.setPointSize(16);
    font.setBold(true);
    font.setLetterSpacing(QFont::PercentageSpacing, 105);
    QColor color = ViThemeManager::color(ViThemeColors::TextColor1);

	//Button
	QObject::connect(mUi->button, SIGNAL(clicked()), this, SLOT(correct()));
	mUi->button->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->button->setText("Process", color, font);
	mUi->button->setSize(140, 60);

	//Label width
	mUi->projectLoader->setStyleSheet(mUi->projectLoader->styleSheet() + "QLabel { width: 150px; min-width: 150px; }");
	mUi->correctionWidget->setStyleSheet(mUi->correctionWidget->styleSheet() + "QLabel { width: 150px; min-width: 150px; }");
}

ViMainCorrectionWidget::~ViMainCorrectionWidget()
{
    delete mUi;
}

void ViMainCorrectionWidget::correct()
{
	ViModifyProcessor *corrector = mUi->correctionWidget->corrector();
	if(corrector != NULL)
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
		engine()->correct(mUi->projectLoader->objects(), corrector);
	}
}

void ViMainCorrectionWidget::showCorrector()
{
	mUi->correctionWidget->show();
	mUi->button->show();
}

void ViMainCorrectionWidget::hideCorrector()
{
	mUi->correctionWidget->hide();
	mUi->button->hide();
}

void ViMainCorrectionWidget::showCorrelation()
{
	/*QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
	ViCorrelationWidget *widget = new ViCorrelationWidget();
	widget->setProject(mUi->projectLoader->project());
	ViStackedWidget::showTemporaryWidget(widget);*/
}

void ViMainCorrectionWidget::hideEvent(QHideEvent *event)
{
    //TODO: clear the project. Currently segfault since the correlation widget is still using it
    //mUi->projectLoader->clear();
}
