#include <vineuralactivationfunctionwidget.h>
#include <ui_vineuralactivationfunctionwidget.h>
#include <viactivationfunctionmanager.h>

ViNeuralActivationFunctionWidget::ViNeuralActivationFunctionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralActivationFunctionWidget();
    mUi->setupUi(this);

	QStringList activationFunctions = ViActivationFunctionManager::names();
	for(int i = 0; i < activationFunctions.size(); ++i)
	{
		mUi->comboBox->addItem(ViName::formatName(activationFunctions[i], "ActivationFunction", true), activationFunctions[i]);
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
