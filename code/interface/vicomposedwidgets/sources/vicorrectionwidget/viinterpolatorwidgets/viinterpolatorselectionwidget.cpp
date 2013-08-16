#include <viinterpolatorselectionwidget.h>
#include <ui_viinterpolatorselectionwidget.h>
#include <viinterpolatormanager.h>

ViInterpolatorSelectionWidget::ViInterpolatorSelectionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViInterpolatorSelectionWidget();
    mUi->setupUi(this);

	QStringList interpolators = ViInterpolatorManager::names();
	for(int i = 0; i < interpolators.size(); ++i)
	{
		mUi->comboBox->addItem(ViName::formatName(interpolators[i], "Interpolator", true), interpolators[i]);
	}

	mDefaultInterpolator = ViInterpolatorManager::defaultName("Interpolator", true);
	mUi->comboBox->setCurrentText(mDefaultInterpolator);
}

ViInterpolatorSelectionWidget::~ViInterpolatorSelectionWidget()
{
    delete mUi;
}

void ViInterpolatorSelectionWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->comboBox->setCurrentText(mDefaultInterpolator);
}

ViInterpolator* ViInterpolatorSelectionWidget::interpolator()
{
	return ViInterpolatorManager::create(mUi->comboBox->itemData(mUi->comboBox->currentIndex()).toString());
}
