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

	//Font
	QFont font;
	font.setFamily("Harabara");
	font.setPointSize(16);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	QColor color = ViThemeManager::color(ViThemeColors::TextColor1);
	
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
	mUi->weightInitializerComboBox->setCurrentIndex(mUi->weightInitializerComboBox->findText(ViWeightInitializerManager::createDefault()->name("WeightInitializer", true), Qt::MatchExactly));

	//Activation function
	QList<ViActivationFunction*> activationFunctions = ViActivationFunctionManager::libraries();
	for(int i = 0; i < activationFunctions.size(); ++i)
	{
		mUi->activationFunctionComboBox->addItem(activationFunctions[i]->name("ActivationFunction", true), activationFunctions[i]->name());
	}
	mUi->activationFunctionComboBox->setCurrentIndex(mUi->activationFunctionComboBox->findText(ViActivationFunctionManager::createDefault()->name("ActivationFunction", true), Qt::MatchExactly));

	//Error functions
	QObject::connect(mUi->errorFunctionAdd, SIGNAL(clicked()), this, SLOT(addErrorFunction()));
	QObject::connect(mUi->errorFunctionRemove, SIGNAL(clicked()), this, SLOT(removeErrorFunction()));
	mUi->errorFunctionAdd->setSize(40, 40);
	mUi->errorFunctionAdd->setText("", color, font);
	mUi->errorFunctionAdd->setIcon(ViThemeManager::icon("add"), 30);
	mUi->errorFunctionRemove->setSize(40, 40);
	mUi->errorFunctionRemove->setText("", color, font);
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
	mUi->trainerComboBox->setCurrentIndex(mUi->trainerComboBox->findText(ViTrainerManager::createDefault()->name("Trainer", true), Qt::MatchExactly));

	//Target providers
	QObject::connect(mUi->targetProviderComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(toggleTargetProvider()));
	QList<ViTargetProvider*> targetProviders = ViTargetProviderManager::libraries();
	for(int i = 0; i < targetProviders.size(); ++i)
	{
		mUi->targetProviderComboBox->addItem(targetProviders[i]->name("TargetProvider", true), targetProviders[i]->name());
	}
	mUi->targetProviderComboBox->setCurrentIndex(mUi->targetProviderComboBox->findText(ViTargetProviderManager::createDefault()->name("TargetProvider", true), Qt::MatchExactly));
	QList<ViInterpolator*> interpolators = ViInterpolatorManager::libraries();
	for(int i = 0; i < interpolators.size(); ++i)
	{
		mUi->interpolatorComboBox->addItem(interpolators[i]->name("Interpolator", true), interpolators[i]->name());
	}
	mUi->interpolatorComboBox->setCurrentIndex(mUi->interpolatorComboBox->findText(ViInterpolatorManager::createDefault()->name("Interpolator", true), Qt::MatchExactly));

	//Button
	QObject::connect(mUi->processButton, SIGNAL(clicked()), this, SLOT(process()));
	mUi->processButton->setIcon(ViThemeManager::icon("startprocess"), 40);
	mUi->processButton->setText("Process", color, font);
	mUi->processButton->setSize(140, 60);
}

ViNeuralSelectorWidget::~ViNeuralSelectorWidget()
{
	delete mUi;

	for(int i = 0; i < mUi->hiddenLayerList->count(); ++i)
	{
		delete mUi->hiddenLayerList->takeItem(i);
	}

	for(int i = 0; i < mUi->errorFunctionList->count(); ++i)
	{
		delete mUi->errorFunctionList->takeItem(i);
	}
}

void ViNeuralSelectorWidget::process()
{
	ViNeuralNetworkFactory factory;

	//Activation function
	factory.setActivationFunction(ViActivationFunctionManager::create(mUi->activationFunctionComboBox->itemData(mUi->activationFunctionComboBox->currentIndex()).toString()));

	//Neurons
	factory.addLayer(mUi->inputSpinBox->value());
	QListWidgetItem *item;
	QPair<int, double> value;
	for(int i = 0; i < mUi->hiddenLayerList->count(); ++i)
	{
		item = mUi->hiddenLayerList->item(i);
		value = item->data(Qt::UserRole).value<QPair<int, double> >();
		if(value.second == 0)
		{
			factory.addLayer(value.first);
		}
		else
		{
			factory.addLayer(value.first, value.second);
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

	//engine()->correct(mUi->projectLoader->currentObject(), new ViNeuralNetworkCorrector(network, trinaer, provider));
}

void ViNeuralSelectorWidget::addHiddenLayer()
{
	QString text = "Hidden Layer " + QString::number(mUi->hiddenLayerList->count() + 1) + " (" + QString::number(mUi->hiddenNeuronsSpinBox->value()) + " neurons, ";
	QPair<int, double> value;
	value.first = mUi->hiddenNeuronsSpinBox->value();
	if(mUi->biasCheckBox->isChecked())
	{
		value.second = mUi->biasValueSpinBox->value();
		text += "bias of " + QString::number(mUi->biasValueSpinBox->value(), 'f', 5);
	}
	else
	{
		value.second = 0;
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
