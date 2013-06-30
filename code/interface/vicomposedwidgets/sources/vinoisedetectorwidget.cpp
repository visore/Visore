#include <vinoisedetectorwidget.h>
#include <ui_vinoisedetectorwidget.h>
#include <vinoisedetectormanager.h>

ViNoiseDetectorWidget::ViNoiseDetectorWidget(QWidget *parent)
    : ViWidget(parent)
{
    mUi = new Ui::ViNoiseDetectorWidget();
    mUi->setupUi(this);

    QObject::connect(mUi->filterAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
    QObject::connect(mUi->filterNoiseRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
    QObject::connect(mUi->correctAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
    QObject::connect(mUi->correctNoiseRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleDetector()));
    toggleDetector();

    QStringList names = ViNoiseDetectorManager::names("", true);
    QString defaultName = ViNoiseDetectorManager::defaultName("", true);
    mUi->detectorComboBox->addItems(names);
    mUi->detectorComboBox->setCurrentText(defaultName);
}

ViNoiseDetectorWidget::~ViNoiseDetectorWidget()
{
    delete mUi;
}

void ViNoiseDetectorWidget::toggleDetector()
{
    if(mUi->filterNoiseRadioButton->isChecked() || mUi->correctNoiseRadioButton->isChecked())
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

ViProcessor::ProcessMode ViNoiseDetectorWidget::processMode()
{
    if(mUi->filterAllRadioButton->isChecked())
    {
        return ViProcessor::All;
    }
    return ViProcessor::Noise;
}

ViModifyProcessor::ModifyMode ViNoiseDetectorWidget::modifyMode()
{
    if(mUi->correctAllRadioButton->isChecked())
    {
        return ViModifyProcessor::All;
    }
    return ViModifyProcessor::Noise;
}

ViNoiseDetector* ViNoiseDetectorWidget::detector()
{
    if(mUi->detectorComboBox->isHidden())
    {
        return NULL;
    }
    return ViNoiseDetectorManager::create(mUi->detectorComboBox->currentText());
}
