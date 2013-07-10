#include <vineuralstructurewidget.h>
#include <ui_vineuralstructurewidget.h>
#include <vineuralnetworkfactory.h>

ViNeuralStructureWidget::ViNeuralStructureWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralStructureWidget();
    mUi->setupUi(this);

	QObject::connect(mUi->biasCheckBox, SIGNAL(toggled(bool)), mUi->biasSpinBox, SLOT(setVisible(bool)));
	QObject::connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addHiddenLayer()));
	QObject::connect(mUi->removeButton, SIGNAL(clicked()), this, SLOT(removeHiddenLayer()));

	mUi->addButton->setIcon(ViThemeManager::icon("add"), 14);
	mUi->removeButton->setIcon(ViThemeManager::icon("remove"), 14);

	addHiddenLayer();
}

ViNeuralStructureWidget::~ViNeuralStructureWidget()
{
	mUi->hiddenList->clear();
    delete mUi;
}

void ViNeuralStructureWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->hiddenList->clear();

	if(mode == ViCorrectionMode::Custom)
	{
		mUi->inputSpinBox->setValue(32);
		mUi->historySpinBox->setValue(6);
		addHiddenLayer(16, 1);
	}
	else if(mode == ViCorrectionMode::Quick)
	{
		mUi->inputSpinBox->setValue(1);
		mUi->historySpinBox->setValue(0);
	}
	else if(mode == ViCorrectionMode::Medium)
	{
		mUi->inputSpinBox->setValue(32);
		mUi->historySpinBox->setValue(6);
		addHiddenLayer(16, 1);
	}
	else if(mode == ViCorrectionMode::Advanced)
	{
		mUi->inputSpinBox->setValue(32);
		mUi->historySpinBox->setValue(8);
		addHiddenLayer(16, 1);
		addHiddenLayer(8, 1);
	}
	else if(mode == ViCorrectionMode::Extreme)
	{
		mUi->inputSpinBox->setValue(64);
		mUi->historySpinBox->setValue(16);
		addHiddenLayer(32, 1);
		addHiddenLayer(16, 1);
	}
}

ViNeuralNetwork* ViNeuralStructureWidget::network()
{
    ViNeuralNetworkFactory factory;

	//Neurons
	factory.setHistory(mUi->historySpinBox->value());
	factory.addLayer(mUi->inputSpinBox->value());
	QListWidgetItem *item;
	QPair<int, double> value;
	for(int i = 0; i < mUi->hiddenList->count(); ++i)
	{
		item = mUi->hiddenList->item(i);
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

	return factory.create();
}

void ViNeuralStructureWidget::addHiddenLayer()
{
    if(mUi->biasCheckBox->isChecked())
	{
		addHiddenLayer(mUi->hiddenSpinBox->value(), mUi->biasSpinBox->value());
	}
	else
	{
		addHiddenLayer(mUi->hiddenSpinBox->value());
    }
}

void ViNeuralStructureWidget::addHiddenLayer(int neurons, double bias)
{
	QString text = "Hidden Layer " + QString::number(mUi->hiddenList->count() + 1) + " (" + QString::number(neurons) + " neurons, ";
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
	mUi->hiddenList->addItem(item);
}

void ViNeuralStructureWidget::removeHiddenLayer()
{
	QList<QListWidgetItem*> items = mUi->hiddenList->selectedItems();
	if(items.isEmpty() && mUi->hiddenList->count() > 0)
	{
		items.append(mUi->hiddenList->item(0));
	}
	for(int i = 0; i < items.size(); ++i)
	{
		delete mUi->hiddenList->takeItem(mUi->hiddenList->row(items[i]));
	}
	QListWidgetItem *item;
	for(int i = 0; i < mUi->hiddenList->count(); ++i)
	{
		item = mUi->hiddenList->item(i);
		item->setText(item->text().replace(QRegExp("Hidden Layer [0-9]+"), "Hidden Layer " + QString::number(i + 1)));
    }
}
