#include <vineuraltrainerwidget.h>
#include <ui_vineuraltrainerwidget.h>
#include <vitrainermanager.h>

ViNeuralTrainerWidget::ViNeuralTrainerWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralTrainerWidget();
    mUi->setupUi(this);

	QStringList trainers = ViTrainerManager::names();
	for(int i = 0; i < trainers.size(); ++i)
	{
		mUi->comboBox->addItem(ViName::formatName(trainers[i], "Trainer", true), trainers[i]);
	}
	mDefaultTrainer = ViTrainerManager::defaultName();
	mUi->comboBox->setCurrentText(mDefaultTrainer);
}

ViNeuralTrainerWidget::~ViNeuralTrainerWidget()
{
    delete mUi;
}

void ViNeuralTrainerWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->comboBox->setCurrentText(mDefaultTrainer);
}

ViTrainer* ViNeuralTrainerWidget::trainer()
{
	return ViTrainerManager::create(mUi->comboBox->itemData(mUi->comboBox->currentIndex()).toString());
}
