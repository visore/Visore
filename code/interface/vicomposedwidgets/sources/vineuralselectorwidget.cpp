#include <vineuralselectorwidget.h>
#include <ui_vineuralselectorwidget.h>

#include <vierrorfunctionmanager.h>

ViNeuralSelectorWidget::ViNeuralSelectorWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViNeuralSelectorWidget();
	mUi->setupUi(this);

	//Font
	QFont font;
	font.setFamily("Harabara");
	font.setPointSize(10);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	QColor color = ViThemeManager::color(ViThemeColors::TextColor1);
	
	//Hidden layers
	QObject::connect(mUi->biasCheckBox, SIGNAL(toggled(bool)), mUi->biasValueSpinBox, SLOT(setVisible(bool)));
	QObject::connect(mUi->biasCheckBox, SIGNAL(toggled(bool)), mUi->biasValueLabel, SLOT(setVisible(bool)));
	QObject::connect(mUi->hiddenLayerAdd, SIGNAL(clicked()), this, SLOT(addHiddenLayer()));
	QObject::connect(mUi->hiddenLayerRemove, SIGNAL(clicked()), mUi->hiddenLayerTable, SLOT(removeSelectedRows()));
	mUi->hiddenLayerAdd->setSize(40, 40);
	mUi->hiddenLayerAdd->setText("", color, font);
	mUi->hiddenLayerAdd->setIcon(ViThemeManager::icon("add"), 30);
	mUi->hiddenLayerRemove->setSize(40, 40);
	mUi->hiddenLayerRemove->setText("", color, font);
	mUi->hiddenLayerRemove->setIcon(ViThemeManager::icon("remove"), 30);

	//Error functions
	QObject::connect(mUi->errorFunctionAdd, SIGNAL(clicked()), this, SLOT(addErrorFunction()));
	QObject::connect(mUi->errorFunctionRemove, SIGNAL(clicked()), mUi->errorFunctionTable, SLOT(removeSelectedRows()));
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
}

ViNeuralSelectorWidget::~ViNeuralSelectorWidget()
{
	delete mUi;
}

void ViNeuralSelectorWidget::addHiddenLayer()
{
	ViTableRow row;
	row.add(mUi->hiddenNeuronsSpinBox->value());
	if(mUi->biasCheckBox->isChecked())
	{
		row.add(mUi->biasValueSpinBox->value());
	}
	else
	{
		row.add("-");
	}
	mUi->hiddenLayerTable->addRow(&row);
}

void ViNeuralSelectorWidget::addErrorFunction()
{
	if(mUi->errorFunctionComboBox->currentText() != "")
	{
		mUi->errorFunctionTable->addRow(mUi->errorFunctionComboBox->currentText());
	}
}
