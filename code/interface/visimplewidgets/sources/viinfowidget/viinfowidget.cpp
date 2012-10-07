#include "viinfowidget.h"
		
ViInfoWidget::ViInfoWidget(QWidget *parent)
	: ViWidget(parent)
{
	mLayout = new QVBoxLayout(this);
	//mLayout->setContentsMargins(0, 0, 0, 0);
}

ViInfoWidget::~ViInfoWidget()
{
	delete mLayout;
	mLayout = NULL;
}

void ViInfoWidget::update()
{
	int maxWidth = 0;
	for(int i = 0; i < mItems.size(); ++i)
	{
		ViInfoLabel *item = mItems[i];
		if(item->hasDescriptor() && item->descriptorWidth() > maxWidth)
		{
			maxWidth = item->descriptorWidth();
		}
	}
	for(int i = 0; i < mItems.size(); ++i)
	{
		ViInfoLabel *item = mItems[i];
		if(item->hasDescriptor())
		{
			item->setDescriptorWidth(maxWidth);
		}
	}
}

ViInfoLabel* ViInfoWidget::addEntry(QString description, QString text)
{
	ViInfoLabel *label = new ViInfoLabel(this, description, text);
	mItems.append(label);
	mLayout->addWidget(label);
	update();
	return label;
}

ViInfoLabel* ViInfoWidget::addEntry(QString text)
{
	return addEntry("", text);
}

void ViInfoWidget::updateEntry(int index, QString description, QString text)
{
	mItems[index]->setDescriptor(description);
	mItems[index]->setText(text);
	update();
}

void ViInfoWidget::updateEntry(int index, QString text)
{
	mItems[index]->setText(text);
	update();
}
