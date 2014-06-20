#include <vimainnoisewidget.h>
#include <ui_vimainnoisewidget.h>
#include <vistackedwidget.h>
#include <vinoisedetectormanager.h>

ViMainNoiseWidget::ViMainNoiseWidget(QWidget *parent)
    : ViWidget(parent)
{
	/*mUi = new Ui::ViMainNoiseWidget();
    mUi->setupUi(this);

	clear();

	mUi->projectLoader->setTypeMode(ViProjectLoader::NoTypes);
	QObject::connect(mUi->projectLoader, SIGNAL(finished()), this, SLOT(showContainer()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(showContainer()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectModeChanged()), this, SLOT(showContainer()));

	//Button
	QObject::connect(mUi->button, SIGNAL(clicked()), this, SLOT(generate()));
	mUi->button->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->button->setText("Process");
	mUi->button->setSize(140, 60);

	//Label width
	QString style = "QLabel { width: 140px; min-width: 140px; }";
	mUi->projectLoader->setStyleSheet(style);
	mUi->container->setStyleSheet(style);

	// Correctors
	QObject::connect(mUi->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(toggleDetector()));
	mUi->typeComboBox->addItem("Custom Mask");
	mUi->typeComboBox->addItem("Noise Mask");

	// Detectors
	QStringList noiseDetectors = ViNoiseDetectorManager::names();
	for(int i = 0; i < noiseDetectors.size(); ++i)
	{
		mUi->detectorComboBox->addItem(ViName::formatName(noiseDetectors[i], "", true), noiseDetectors[i]);
	}
	mDefaultDetector = ViNoiseDetectorManager::defaultName("", true);
	mUi->detectorComboBox->setCurrentText(mDefaultDetector);*/
}

ViMainNoiseWidget::~ViMainNoiseWidget()
{
	clear();
    delete mUi;
}

void ViMainNoiseWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->container->hide();
	mUi->detectorLabel->hide();
	mUi->detectorComboBox->hide();
	mUi->detectorComboBox->setCurrentText(mDefaultDetector);
}

void ViMainNoiseWidget::showContainer()
{
	mUi->container->setVisible(!mUi->projectLoader->objects().isEmpty());
}

void ViMainNoiseWidget::toggleDetector()
{
	if(mUi->typeComboBox->currentText() == "Noise Mask")
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

void ViMainNoiseWidget::generate()
{
	/*if(mUi->typeComboBox->currentText() == "Custom Mask")
	{
		engine()->generateCustomMask(mUi->projectLoader->objects());
	}
	else if(mUi->typeComboBox->currentText() == "Noise Mask")
	{
		engine()->generateNoiseMask(mUi->projectLoader->objects(), ViNoiseDetectorManager::create(mUi->detectorComboBox->itemData(mUi->detectorComboBox->currentIndex()).toString()));
	}*/
}
