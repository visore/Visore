#include <vineuraltrainerwidget.h>
#include <ui_vineuraltrainerwidget.h>
#include <vitrainermanager.h>

ViNeuralTrainerWidget::ViNeuralTrainerWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralTrainerWidget();
    mUi->setupUi(this);

	QList<ViTrainer*> trainers = ViTrainerManager::libraries();
	for(int i = 0; i < trainers.size(); ++i)
	{
		mUi->comboBox->addItem(trainers[i]->name("Trainer", true), trainers[i]->name());
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
