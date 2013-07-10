#include <vineuralerrorfunctionwidget.h>
#include <ui_vineuralerrorfunctionwidget.h>
#include <vierrorfunctionmanager.h>

ViNeuralErrorFunctionWidget::ViNeuralErrorFunctionWidget(QWidget *parent)
    : ViWidget(parent)
{
	mUi = new Ui::ViNeuralErrorFunctionWidget();
    mUi->setupUi(this);

	QObject::connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addErrorFunction()));
	QObject::connect(mUi->removeButton, SIGNAL(clicked()), this, SLOT(removeErrorFunction()));
	mUi->addButton->setIcon(ViThemeManager::icon("add"), 14);
	mUi->removeButton->setIcon(ViThemeManager::icon("remove"), 14);

	QList<ViErrorFunction*> errorFunctions = ViErrorFunctionManager::libraries();
	for(int i = 0; i < errorFunctions.size(); ++i)
	{
		mUi->comboBox->addItem(errorFunctions[i]->name("Function", true), errorFunctions[i]->name());
	}
	mDefaultFunction = ViErrorFunctionManager::defaultName("Function", true);
	mUi->comboBox->setCurrentText(mDefaultFunction);
	mUi->comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

ViNeuralErrorFunctionWidget::~ViNeuralErrorFunctionWidget()
{
	mUi->listWidget->clear();
    delete mUi;
}

void ViNeuralErrorFunctionWidget::setMode(ViCorrectionMode::Mode mode)
{
	mUi->listWidget->clear();
	mUi->comboBox->setCurrentText(mDefaultFunction);
}

QList<ViErrorFunction*> ViNeuralErrorFunctionWidget::errorFunctions()
{
	QList<ViErrorFunction*> functions;
	for(int i = 0; i < mUi->listWidget->count(); ++i)
	{
		functions.append(ViErrorFunctionManager::create(mUi->listWidget->item(i)->data(Qt::UserRole).toString()));
	}
	return functions;
}

void ViNeuralErrorFunctionWidget::addErrorFunction()
{
	QString text = mUi->comboBox->currentText();
	if(text != "" && mUi->listWidget->findItems(text, Qt::MatchExactly).isEmpty())
	{
		QListWidgetItem *item = new QListWidgetItem(text);
		item->setData(Qt::UserRole, mUi->comboBox->itemData(mUi->comboBox->currentIndex()));
		mUi->listWidget->addItem(item);
	}
}

void ViNeuralErrorFunctionWidget::removeErrorFunction()
{
	QList<QListWidgetItem*> items = mUi->listWidget->selectedItems();
	if(items.isEmpty() && mUi->listWidget->count() > 0)
	{
		items.append(mUi->listWidget->item(0));
	}
	for(int i = 0; i < items.size(); ++i)
	{
		delete mUi->listWidget->takeItem(mUi->listWidget->row(items[i]));
	}
}
