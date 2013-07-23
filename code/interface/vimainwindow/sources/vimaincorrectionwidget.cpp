#include <vimaincorrectionwidget.h>
#include <ui_vimaincorrectionwidget.h>
#include <vistackedwidget.h>
#include <vimaincorrelationwidget.h>

ViMainCorrectionWidget::ViMainCorrectionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViMainCorrectionWidget();
    mUi->setupUi(this);

	clear();

	mUi->projectLoader->setTypeMode(ViProjectLoader::NoTypes);
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), mUi->container, SLOT(show()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectModeChanged()), mUi->container, SLOT(show()));

	//Button
	QObject::connect(mUi->button, SIGNAL(clicked()), this, SLOT(correct()));
	mUi->button->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->button->setText("Process");
	mUi->button->setSize(140, 60);

	//Label width
	QString style = "QLabel { width: 140px; min-width: 140px; }";
	mUi->projectLoader->setStyleSheet(style);
	mUi->container->setStyleSheet(style);

	// Correctors
	mUi->correctorComboBox->addItems(mUi->correctionWidget->correctors());
	QObject::connect(mUi->correctorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCorrector()));
	changeCorrector();

	// Mode
	mUi->modeComboBox->addItems(ViCorrectionMode::modes());
	mUi->modeComboBox->setCurrentText(ViCorrectionMode::defaultMode());
	QObject::connect(mUi->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMode()));
	changeMode();
}

ViMainCorrectionWidget::~ViMainCorrectionWidget()
{
	clear();
    delete mUi;
}

void ViMainCorrectionWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->correctionWidget->clear();
	mUi->container->hide();
}

void ViMainCorrectionWidget::changeCorrector()
{
	mUi->correctionWidget->changeCorrector(mUi->correctorComboBox->currentText());
}

void ViMainCorrectionWidget::changeMode()
{
	mUi->correctionWidget->changeMode(ViCorrectionMode::stringToMode(mUi->modeComboBox->currentText()));
}

void ViMainCorrectionWidget::correct()
{
	ViModifyProcessor *corrector = mUi->correctionWidget->corrector();
	if(corrector != NULL)
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
		engine()->correct(mUi->projectLoader->objects(), corrector, mUi->correlateCheckBox->isChecked());
	}
}

void ViMainCorrectionWidget::showCorrelation()
{
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(showCorrelation()));
	if(mUi->correlateCheckBox->isChecked())
	{
		ViMainCorrelationWidget *widget = dynamic_cast<ViMainCorrelationWidget*>(ViStackedWidget::widget("ViMainCorrelationWidget"));
		if(widget != NULL)
		{
			widget->setData(mUi->projectLoader->takeProject(), mUi->projectLoader->objects());
			clear();
			ViStackedWidget::setCurrentWidget(widget);
		}
	}
}
