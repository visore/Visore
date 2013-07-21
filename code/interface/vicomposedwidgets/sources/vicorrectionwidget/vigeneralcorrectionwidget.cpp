#include <vigeneralcorrectionwidget.h>
#include <ui_vigeneralcorrectionwidget.h>
#include <vinoisedetectormanager.h>

ViGeneralCorrectionWidget::ViGeneralCorrectionWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViGeneralCorrectionWidget();
	mUi->setupUi(this);

	QObject::connect(mUi->processAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
	QObject::connect(mUi->processNoisyRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
	QObject::connect(mUi->correctAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
	QObject::connect(mUi->correctNoisyRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));

	QStringList noiseDetectors = ViNoiseDetectorManager::names();
	for(int i = 0; i < noiseDetectors.size(); ++i)
	{
		mUi->detectorComboBox->addItem(ViName::formatName(noiseDetectors[i], "", true), noiseDetectors[i]);
	}
	mDefaultDetector = ViNoiseDetectorManager::defaultName("", true);
	mUi->detectorComboBox->setCurrentText(mDefaultDetector);
}

ViGeneralCorrectionWidget::~ViGeneralCorrectionWidget()
{
	delete mUi;
}
void ViGeneralCorrectionWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->separateRadioButton->setChecked(true);
	mUi->processAllRadioButton->setChecked(true);
	mUi->correctNoisyRadioButton->setChecked(true);
	mUi->detectorComboBox->setCurrentText(mDefaultDetector);
}

ViProcessor::ChannelMode ViGeneralCorrectionWidget::channelMode()
{
	if(mUi->separateRadioButton->isChecked())
	{
		return ViProcessor::Separated;
	}
	return ViProcessor::Combined;
}

ViProcessor::ProcessMode ViGeneralCorrectionWidget::processMode()
{
	if(mUi->processAllRadioButton->isChecked())
	{
		return ViProcessor::All;
	}
	return ViProcessor::Noise;
}

ViModifyProcessor::ModifyMode ViGeneralCorrectionWidget::modifyMode()
{
	if(mUi->correctAllRadioButton->isChecked())
	{
		return ViModifyProcessor::All;
	}
	return ViModifyProcessor::Noise;
}

ViNoiseDetector* ViGeneralCorrectionWidget::noiseDetector()
{
	return ViNoiseDetectorManager::create(mUi->detectorComboBox->itemData(mUi->detectorComboBox->currentIndex()).toString());
}

void ViGeneralCorrectionWidget::toggleDetector()
{
	if(mUi->processNoisyRadioButton->isChecked() || mUi->correctNoisyRadioButton->isChecked())
	{
		mUi->detectorLabel->show();
		mUi->detectorComboBox->show();
	}
	else
	{
		mUi->detectorLabel->hide();
		mUi->detectorComboBox->hide();
	}
}
