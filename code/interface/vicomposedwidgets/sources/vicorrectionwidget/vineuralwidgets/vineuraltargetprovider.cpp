#include <vineuraltargetproviderwidget.h>
#include <ui_vineuraltargetproviderwidget.h>
#include <vitargetprovidermanager.h>
#include <viinterpolatormanager.h>
#include <viinterpolationtargetprovider.h>

ViNeuralTargetProviderWidget::ViNeuralTargetProviderWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralTargetProviderWidget();
    mUi->setupUi(this);

	QObject::connect(mUi->providerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(toggleProvider()));
	QList<ViTargetProvider*> targetProviders = ViTargetProviderManager::libraries();
	for(int i = 0; i < targetProviders.size(); ++i)
	{
		mUi->providerComboBox->addItem(targetProviders[i]->name("TargetProvider", true), targetProviders[i]->name());
	}
	mDefaultProvider = ViTargetProviderManager::defaultName("TargetProvider", true);
	mUi->providerComboBox->setCurrentText(mDefaultProvider);

	QList<ViInterpolator*> interpolators = ViInterpolatorManager::libraries();
	for(int i = 0; i < interpolators.size(); ++i)
	{
		mUi->interpolatorComboBox->addItem(interpolators[i]->name("Interpolator", true), interpolators[i]->name());
	}
	mDefaultInterpolator = ViInterpolatorManager::defaultName("Interpolator", true);
	mUi->interpolatorComboBox->setCurrentText(mDefaultInterpolator);
}

ViNeuralTargetProviderWidget::~ViNeuralTargetProviderWidget()
{
    delete mUi;
}

void ViNeuralTargetProviderWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->providerComboBox->setCurrentText(mDefaultProvider);
	mUi->interpolatorComboBox->setCurrentText(mDefaultInterpolator);
}

ViTargetProvider* ViNeuralTargetProviderWidget::targetProvider()
{
	QString providerChoice = mUi->providerComboBox->itemData(mUi->providerComboBox->currentIndex()).toString();
	ViTargetProvider *provider = ViTargetProviderManager::create(providerChoice);
	if(providerChoice.contains("Interpolation"))
	{
		((ViInterpolationTargetProvider*) provider)->setInterpolator(ViInterpolatorManager::create(mUi->interpolatorComboBox->itemData(mUi->interpolatorComboBox->currentIndex()).toString()));
	}
	return provider;
}

void ViNeuralTargetProviderWidget::toggleProvider()
{
	if(mUi->providerComboBox->itemData(mUi->providerComboBox->currentIndex()).toString().contains("Interpolation"))
	{
		mUi->interpolatorLabel->show();
		mUi->interpolatorComboBox->show();
	}
	else
	{
		mUi->interpolatorLabel->hide();
		mUi->interpolatorComboBox->hide();
	}
}
