#include "viinfolabel.h"

ViInfoLabel::ViInfoLabel(QWidget *parent, QString description, QString text)
	: ViWidget(parent)
{
	mLayout = new QHBoxLayout(this);
	mLayout->setContentsMargins(0, 0, 0, 0);
	mLayout->setSpacing(0);
	mDescriptorLabel = new QLabel();
	mLayout->addWidget(mDescriptorLabel);
	mTextLabel = new QLabel();
	mLayout->addWidget(mTextLabel);
	mLayout->addWidget(mTextLabel);
	setText(description, text);
}

ViInfoLabel::ViInfoLabel(QWidget *parent, QString text)
{
	ViInfoLabel(parent, "", text);
}

ViInfoLabel::ViInfoLabel(QString description, QString text)
{
	ViInfoLabel(0, description, text);
}

ViInfoLabel::ViInfoLabel(QString text)
{
	ViInfoLabel(0, "", text);
}

ViInfoLabel::~ViInfoLabel()
{
	delete mLayout;
	mLayout = NULL;
}

void ViInfoLabel::setText(QString description, QString text)
{
	setDescriptor(description);
	setText(text);
}

void ViInfoLabel::setText(QString text)
{
	mTextLabel->setText(text);
}

void ViInfoLabel::setDescriptor(QString description)
{
	if(description != "")
	{
		description += ": ";
	}
	else
	{
		setDescriptorWidth(0);
	}
	mDescriptorLabel->setText(description);
}
		
int ViInfoLabel::textWidth()
{
	return mTextLabel->fontMetrics().width(mTextLabel->text());
}

int ViInfoLabel::descriptorWidth()
{
	return mTextLabel->fontMetrics().width(mDescriptorLabel->text());
}

void ViInfoLabel::setTextWidth(int width)
{
	mTextLabel->setMaximumWidth(width);
	mTextLabel->setMinimumWidth(width);
}

void ViInfoLabel::setDescriptorWidth(int width)
{
	mDescriptorLabel->setMaximumWidth(width);
	mDescriptorLabel->setMinimumWidth(width);
}

bool ViInfoLabel::hasDescriptor()
{
	return mDescriptorLabel->text() != "";
}
