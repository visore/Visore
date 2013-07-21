#include <vineuralweightinitializerwidget.h>
#include <ui_vineuralweightinitializerwidget.h>
#include <viweightinitializermanager.h>

ViNeuralWeightInitializerWidget::ViNeuralWeightInitializerWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralWeightInitializerWidget();
    mUi->setupUi(this);

	QStringList weightInitializers = ViWeightInitializerManager::names();
	for(int i = 0; i < weightInitializers.size(); ++i)
	{
		mUi->comboBox->addItem(ViName::formatName(weightInitializers[i], "WeightInitializer", true), weightInitializers[i]);
	}
	mDefaultInitializer = ViWeightInitializerManager::defaultName("WeightInitializer", true);
	mUi->comboBox->setCurrentText(mDefaultInitializer);
}

ViNeuralWeightInitializerWidget::~ViNeuralWeightInitializerWidget()
{
    delete mUi;
}

void ViNeuralWeightInitializerWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->comboBox->setCurrentText(mDefaultInitializer);
}

ViWeightInitializer* ViNeuralWeightInitializerWidget::weightInitializer()
{
	return ViWeightInitializerManager::create(mUi->comboBox->itemData(mUi->comboBox->currentIndex()).toString());
}
