#include <vineuralselectorwidget.h>
#include <ui_vineuralselectorwidget.h>

#include <vineuralnetworkfactory.h>
#include <viweightinitializermanager.h>
#include <vierrorfunctionmanager.h>
#include <viactivationfunctionmanager.h>
#include <vitrainermanager.h>
#include <vitargetprovidermanager.h>
#include <viinterpolatormanager.h>
#include <viinterpolationtargetprovider.h>

ViNeuralSelectorWidget::ViNeuralSelectorWidget(QWidget *parent)
    : ViWidget(parent)
{
    mUi = new Ui::ViNeuralSelectorWidget();
    mUi->setupUi(this);

	//Mode
    mUi->modeComboBox->addItem("Custom Correction");
    mUi->modeComboBox->addItem("Quick Correction");
	mUi->modeComboBox->addItem("Medium Correction");
	mUi->modeComboBox->addItem("Advanced Correction");
	mUi->modeComboBox->addItem("Extreme Correction");
	QObject::connect(mUi->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSettings(int)));
	
	//Hidden layers
	QObject::connect(mUi->biasCheckBox, SIGNAL(toggled(bool)), mUi->biasValueSpinBox, SLOT(setVisible(bool)));
	QObject::connect(mUi->biasCheckBox, SIGNAL(toggled(bool)), mUi->biasValueLabel, SLOT(setVisible(bool)));
	QObject::connect(mUi->hiddenLayerAdd, SIGNAL(clicked()), this, SLOT(addHiddenLayer()));
	QObject::connect(mUi->hiddenLayerRemove, SIGNAL(clicked()), this, SLOT(removeHiddenLayer()));
	mUi->hiddenLayerAdd->setSize(40, 40);
	mUi->hiddenLayerAdd->setIcon(ViThemeManager::icon("add"), 30);
	mUi->hiddenLayerRemove->setSize(40, 40);
	mUi->hiddenLayerRemove->setIcon(ViThemeManager::icon("remove"), 30);

	//Weights
	QList<ViWeightInitializer*> weightInitializers = ViWeightInitializerManager::libraries();
	for(int i = 0; i < weightInitializers.size(); ++i)
	{
		mUi->weightInitializerComboBox->addItem(weightInitializers[i]->name("WeightInitializer", true), weightInitializers[i]->name());
	}

	//Activation function
	QList<ViActivationFunction*> activationFunctions = ViActivationFunctionManager::libraries();
	for(int i = 0; i < activationFunctions.size(); ++i)
	{
		mUi->activationFunctionComboBox->addItem(activationFunctions[i]->name("ActivationFunction", true), activationFunctions[i]->name());
	}

	//Error functions
	QObject::connect(mUi->errorFunctionAdd, SIGNAL(clicked()), this, SLOT(addErrorFunction()));
	QObject::connect(mUi->errorFunctionRemove, SIGNAL(clicked()), this, SLOT(removeErrorFunction()));
	mUi->errorFunctionAdd->setSize(40, 40);
	mUi->errorFunctionAdd->setIcon(ViThemeManager::icon("add"), 30);
	mUi->errorFunctionRemove->setSize(40, 40);
	mUi->errorFunctionRemove->setIcon(ViThemeManager::icon("remove"), 30);
	QList<ViErrorFunction*> errorFunctions = ViErrorFunctionManager::libraries();
	for(int i = 0; i < errorFunctions.size(); ++i)
	{
		mUi->errorFunctionComboBox->addItem(errorFunctions[i]->name("Function", true), errorFunctions[i]->name());
	}

	//Trainers
	QList<ViTrainer*> trainers = ViTrainerManager::libraries();
	for(int i = 0; i < trainers.size(); ++i)
	{
		mUi->trainerComboBox->addItem(trainers[i]->name("Trainer", true), trainers[i]->name());
	}

	//Target providers
	QObject::connect(mUi->targetProviderComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(toggleTargetProvider()));
	QList<ViTargetProvider*> targetProviders = ViTargetProviderManager::libraries();
	for(int i = 0; i < targetProviders.size(); ++i)
	{
		mUi->targetProviderComboBox->addItem(targetProviders[i]->name("TargetProvider", true), targetProviders[i]->name());
	}
	QList<ViInterpolator*> interpolators = ViInterpolatorManager::libraries();
	for(int i = 0; i < interpolators.size(); ++i)
	{
		mUi->interpolatorComboBox->addItem(interpolators[i]->name("Interpolator", true), interpolators[i]->name());
	}

    changeSettings(0);
}

ViNeuralSelectorWidget::~ViNeuralSelectorWidget()
{
	for(int i = 0; i < mUi->hiddenLayerList->count(); ++i)
	{
		delete mUi->hiddenLayerList->takeItem(i);
	}

	for(int i = 0; i < mUi->errorFunctionList->count(); ++i)
	{
		delete mUi->errorFunctionList->takeItem(i);
    }

    delete mUi;
}

ViNeuralCorrector* ViNeuralSelectorWidget::corrector()
{
    ViNeuralNetworkFactory factory;

	//Activation function
	factory.setActivationFunction(ViActivationFunctionManager::create(mUi->activationFunctionComboBox->itemData(mUi->activationFunctionComboBox->currentIndex()).toString()));

	//Neurons
	factory.setHistory(mUi->historySpinBox->value());
	factory.addLayer(mUi->inputSpinBox->value());
	QListWidgetItem *item;
	QPair<int, double> value;
	for(int i = 0; i < mUi->hiddenLayerList->count(); ++i)
	{
		item = mUi->hiddenLayerList->item(i);
		value = item->data(Qt::UserRole).value<QPair<int, double> >();
		if(value.first > 0)
		{
			if(value.second == 0)
			{
				factory.addLayer(value.first);
			}
			else
			{
				factory.addLayer(value.first, value.second);
			}
		}
	}
	factory.addLayer(1);

	//Trainer
	ViTrainer *trainer = ViTrainerManager::create(mUi->trainerComboBox->itemData(mUi->trainerComboBox->currentIndex()).toString());

	//Error functions
	for(int i = 0; i < mUi->errorFunctionList->count(); ++i)
	{
		trainer->addErrorFunction(ViErrorFunctionManager::create(mUi->errorFunctionList->item(i)->data(Qt::UserRole).toString()));		
	}

	//Weight initializer
	ViWeightInitializer *weightInitializer = ViWeightInitializerManager::create(mUi->weightInitializerComboBox->itemData(mUi->weightInitializerComboBox->currentIndex()).toString());
	ViNeuralNetwork *network = factory.create();
	weightInitializer->initialize(network, trainer->learningRate());
	delete weightInitializer;

	//Target provider
	QString providerChoice = mUi->targetProviderComboBox->itemData(mUi->targetProviderComboBox->currentIndex()).toString();
	ViTargetProvider *provider = ViTargetProviderManager::create(providerChoice);
	if(providerChoice.contains("Interpolation"))
	{
		((ViInterpolationTargetProvider*) provider)->setInterpolator(ViInterpolatorManager::create(mUi->interpolatorComboBox->itemData(mUi->interpolatorComboBox->currentIndex()).toString()));
	}

	ViNeuralCorrector *corrector = new ViNeuralCorrector(network, trainer, provider);
	
	//Seperate channels
	corrector->enableSeparateChannels(mUi->channelsCheckBox->isChecked());

    //Noise detector
    corrector->setProcessMode(mUi->noiseDetectorWidget->processMode());
    corrector->setModifyMode(mUi->noiseDetectorWidget->modifyMode());
    corrector->setNoiseDetector(mUi->noiseDetectorWidget->detector());

    return corrector;
}

void ViNeuralSelectorWidget::addHiddenLayer()
{
    if(mUi->biasCheckBox->isChecked())
	{
		addHiddenLayer(mUi->hiddenNeuronsSpinBox->value(), mUi->biasValueSpinBox->value());
	}
	else
	{
		addHiddenLayer(mUi->hiddenNeuronsSpinBox->value());
    }
}

void ViNeuralSelectorWidget::addHiddenLayer(int neurons, double bias)
{
    QString text = "Hidden Layer " + QString::number(mUi->hiddenLayerList->count() + 1) + " (" + QString::number(neurons) + " neurons, ";
	QPair<int, double> value;
	value.first = neurons;
	value.second = bias;
	if(bias)
	{
		text += "bias of " + QString::number(bias, 'f', 5);
	}
	else
	{
		text += "no bias";
	}
	text += ")";
	QListWidgetItem *item = new QListWidgetItem(text);
	item->setData(Qt::UserRole, QVariant::fromValue(value));
    mUi->hiddenLayerList->addItem(item);
}

void ViNeuralSelectorWidget::removeHiddenLayer()
{
    QList<QListWidgetItem*> items = mUi->hiddenLayerList->selectedItems();
	for(int i = 0; i < items.size(); ++i)
	{
		delete mUi->hiddenLayerList->takeItem(mUi->hiddenLayerList->row(items[i]));
	}
	QListWidgetItem *item;
	for(int i = 0; i < mUi->hiddenLayerList->count(); ++i)
	{
		item = mUi->hiddenLayerList->item(i);
		item->setText(item->text().replace(QRegExp("Hidden Layer [0-9]+"), "Hidden Layer " + QString::number(i + 1)));
    }
}

void ViNeuralSelectorWidget::addErrorFunction()
{
    QString text = mUi->errorFunctionComboBox->currentText();
	if(text != "" && mUi->errorFunctionList->findItems(text, Qt::MatchExactly).isEmpty())
	{
		QListWidgetItem *item = new QListWidgetItem(text);
		item->setData(Qt::UserRole, mUi->errorFunctionComboBox->itemData(mUi->errorFunctionComboBox->currentIndex()));
		mUi->errorFunctionList->addItem(item);
    }
}

void ViNeuralSelectorWidget::removeErrorFunction()
{
    QList<QListWidgetItem*> items = mUi->errorFunctionList->selectedItems();
	for(int i = 0; i < items.size(); ++i)
	{
		delete mUi->errorFunctionList->takeItem(mUi->errorFunctionList->row(items[i]));
    }
}

void ViNeuralSelectorWidget::toggleTargetProvider()
{
    if(mUi->targetProviderComboBox->itemData(mUi->targetProviderComboBox->currentIndex()).toString().contains("Interpolation"))
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

void ViNeuralSelectorWidget::changeSettings(int index)
{
    if(index == 0)
	{
		mUi->inputsGroupBox->show();
		mUi->hiddenLayersGroupBox->show();
		mUi->weightsGroupBox->show();
		mUi->activationFunctionGroupBox->show();
		mUi->errorFunctionsGroupBox->show();
		mUi->trainerGroupBox->show();
		mUi->targetGroupBox->show();
		mUi->channelsGroupBox->show();
        mUi->noiseGroupBox->show();
	}
	else
	{
		mUi->inputsGroupBox->hide();
		mUi->hiddenLayersGroupBox->hide();
		mUi->weightsGroupBox->hide();
		mUi->activationFunctionGroupBox->hide();
		mUi->errorFunctionsGroupBox->hide();
		mUi->trainerGroupBox->hide();
		mUi->targetGroupBox->hide();
		mUi->channelsGroupBox->hide();
        mUi->noiseGroupBox->hide();
	}

	int weightIndex = mUi->weightInitializerComboBox->findText(ViWeightInitializerManager::createDefault()->name("WeightInitializer", true), Qt::MatchExactly);
	int activationIndex = mUi->activationFunctionComboBox->findText(ViActivationFunctionManager::createDefault()->name("ActivationFunction", true), Qt::MatchExactly);
	int trainerIndex = mUi->trainerComboBox->findText(ViTrainerManager::createDefault()->name("Trainer", true), Qt::MatchExactly);
	int targetIndex = mUi->targetProviderComboBox->findText(ViTargetProviderManager::createDefault()->name("TargetProvider", true), Qt::MatchExactly);
	int interpolatorIndex = mUi->interpolatorComboBox->findText(ViInterpolatorManager::createDefault()->name("Interpolator", true), Qt::MatchExactly);

	if(index == 0)
	{
		mUi->inputSpinBox->setValue(32);
		mUi->historySpinBox->setValue(8);
		mUi->hiddenNeuronsSpinBox->setValue(16);
		mUi->biasCheckBox->setChecked(true);
		mUi->biasValueSpinBox->setValue(1);
		mUi->hiddenLayerList->clear();
		addHiddenLayer(mUi->hiddenNeuronsSpinBox->value(), mUi->biasCheckBox->isChecked() * mUi->biasValueSpinBox->value());
		mUi->weightInitializerComboBox->setCurrentIndex(weightIndex);
		mUi->activationFunctionComboBox->setCurrentIndex(activationIndex);
		mUi->errorFunctionList->clear();
		mUi->trainerComboBox->setCurrentIndex(trainerIndex);
		mUi->targetProviderComboBox->setCurrentIndex(targetIndex);
		mUi->interpolatorComboBox->setCurrentIndex(interpolatorIndex);
		mUi->channelsCheckBox->setChecked(true);
	}
	else if(index == 1)
	{
		mUi->inputSpinBox->setValue(1);
		mUi->historySpinBox->setValue(0);
		mUi->hiddenLayerList->clear();
		mUi->weightInitializerComboBox->setCurrentIndex(weightIndex);
		mUi->activationFunctionComboBox->setCurrentIndex(activationIndex);
		mUi->errorFunctionList->clear();
		mUi->trainerComboBox->setCurrentIndex(trainerIndex);
		mUi->targetProviderComboBox->setCurrentIndex(targetIndex);
		mUi->interpolatorComboBox->setCurrentIndex(interpolatorIndex);
		mUi->channelsCheckBox->setChecked(true);
	}
	else if(index == 2)
	{
		mUi->inputSpinBox->setValue(32);
		mUi->historySpinBox->setValue(4);
		mUi->hiddenLayerList->clear();
		addHiddenLayer(16, 1);
		mUi->weightInitializerComboBox->setCurrentIndex(weightIndex);
		mUi->activationFunctionComboBox->setCurrentIndex(activationIndex);
		mUi->errorFunctionList->clear();
		mUi->trainerComboBox->setCurrentIndex(trainerIndex);
		mUi->targetProviderComboBox->setCurrentIndex(targetIndex);
		mUi->interpolatorComboBox->setCurrentIndex(interpolatorIndex);
		mUi->channelsCheckBox->setChecked(true);
	}
	else if(index == 3)
	{
		mUi->inputSpinBox->setValue(128);
		mUi->historySpinBox->setValue(32);
		mUi->hiddenLayerList->clear();
		addHiddenLayer(32, 1);
		addHiddenLayer(16, 1);
		mUi->weightInitializerComboBox->setCurrentIndex(weightIndex);
		mUi->activationFunctionComboBox->setCurrentIndex(activationIndex);
		mUi->errorFunctionList->clear();
		mUi->trainerComboBox->setCurrentIndex(trainerIndex);
		mUi->targetProviderComboBox->setCurrentIndex(targetIndex);
		mUi->interpolatorComboBox->setCurrentIndex(interpolatorIndex);
		mUi->channelsCheckBox->setChecked(true);
	}
	else if(index == 4)
	{
		mUi->inputSpinBox->setValue(256);
		mUi->historySpinBox->setValue(64);
		mUi->hiddenLayerList->clear();
		addHiddenLayer(64, 1);
		addHiddenLayer(32, 1);
		mUi->weightInitializerComboBox->setCurrentIndex(weightIndex);
		mUi->activationFunctionComboBox->setCurrentIndex(activationIndex);
		mUi->errorFunctionList->clear();
		mUi->trainerComboBox->setCurrentIndex(trainerIndex);
		mUi->targetProviderComboBox->setCurrentIndex(targetIndex);
		mUi->interpolatorComboBox->setCurrentIndex(interpolatorIndex);
		mUi->channelsCheckBox->setChecked(true);
    }
}
