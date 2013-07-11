#include <vineuralactivationfunctionwidget.h>
#include <ui_vineuralactivationfunctionwidget.h>
#include <viactivationfunctionmanager.h>

ViNeuralActivationFunctionWidget::ViNeuralActivationFunctionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralActivationFunctionWidget();
    mUi->setupUi(this);

	QList<ViActivationFunction*> activationFunctions = ViActivationFunctionManager::libraries();
	for(int i = 0; i < activationFunctions.size(); ++i)
	{
		mUi->comboBox->addItem(activationFunctions[i]->name("ActivationFunction", true), activationFunctions[i]->name());
	}

	mDefaultFunction = "";
	for(int i = 0; i < mUi->comboBox->count(); ++i)
	{
		if(mUi->comboBox->itemText(i) == "Sigmoid")
		{
			mDefaultFunction = "Sigmoid";
			break;
		}
	}
	if(mDefaultFunction == "")
	{
		mDefaultFunction = ViActivationFunctionManager::defaultName("ActivationFunction", true);
	}
	mUi->comboBox->setCurrentText(mDefaultFunction);
}

ViNeuralActivationFunctionWidget::~ViNeuralActivationFunctionWidget()
{
    delete mUi;
}

void ViNeuralActivationFunctionWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->comboBox->setCurrentText(mDefaultFunction);
}

ViActivationFunction* ViNeuralActivationFunctionWidget::activationFunction()
{
	return ViActivationFunctionManager::create(mUi->comboBox->itemData(mUi->comboBox->currentIndex()).toString());
}
