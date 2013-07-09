#include <vicorrectionwidget.h>
#include <ui_vicorrectionwidget.h>
#include <vitabwidget.h>

#include <vineuralstructurewidget.h>

ViCorrectionWidget::ViCorrectionWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViCorrectionWidget();
	mUi->setupUi(this);

	// Mode
	mUi->modeComboBox->addItems(ViCorrectionMode::modes());
	mUi->modeComboBox->setCurrentText(ViCorrectionMode::defaultMode());

	// Neural Corrector
	mUi->correctorComboBox->addItem("Neural Corrector");
	ViTabWidget *neuralTabWidget = new ViTabWidget();
	neuralTabWidget->addTab(new ViNeuralStructureWidget(), "Neural Structure");
	mUi->stackedWidget->addWidget(neuralTabWidget);

	QObject::connect(mUi->correctorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectCorrector()));
	QObject::connect(mUi->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMode()));

	selectCorrector();
}

ViCorrectionWidget::~ViCorrectionWidget()
{
	delete mUi;
}

void ViCorrectionWidget::selectCorrector()
{
	QString corrector = mUi->correctorComboBox->currentText();
	if(corrector == "Neural Corrector")
	{
		mUi->stackedWidget->setCurrentIndex(0);
	}
	else
	{
		LOG("Invalid corrector selected.", QtCriticalMsg);
	}
	selectMode();
}

void ViCorrectionWidget::selectMode()
{
	ViCorrectionMode::Mode mode = ViCorrectionMode::stringToMode(mUi->modeComboBox->currentText());
	int count1 = mUi->stackedWidget->count(), count2;
	ViTabWidget *tabWidget;
	ViCorrectionMode *modeWidget;
	for(int i = 0; i < count1; ++i)
	{
		tabWidget = dynamic_cast<ViTabWidget*>(mUi->stackedWidget->widget(i));
		if(tabWidget != NULL)
		{
			count2 = tabWidget->count();
			for(int j = 0; j < count2; ++j)
			{
				modeWidget = dynamic_cast<ViCorrectionMode*>(tabWidget->widget(j));
				if(modeWidget != NULL)
				{
					modeWidget->setMode(mode);
				}
			}
		}
	}
}
