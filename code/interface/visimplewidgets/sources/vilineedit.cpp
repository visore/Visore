#include "vilineedit.h"
#include "vithememanager.h"

ViLineEdit::ViLineEdit(QWidget *parent)
	: ViWidget(parent)
{
	mLayout.setContentsMargins(0, 0, 0, 0);
	setLayout(&mLayout);

	mLineEdit = new QLineEdit(this);
	mLayout.addWidget(mLineEdit);

	setStyleSheet("\
		QLineEdit:focus\
		{\
			border-radius: 3px;\
			border: 1px solid " + ViThemeManager::color(13).name() + ";\
		}\
	");
}

void ViLineEdit::pad(ViLineEdit::Direction direction, int width)
{
	QMargins margins = mLineEdit->textMargins();
	if(direction == ViLineEdit::Left)
	{
		margins.setLeft(width);
	}
	else if(direction == ViLineEdit::Right)
	{
		margins.setRight(width);
	}
	mLineEdit->setTextMargins(margins);
	adjustSize();
}

void ViLineEdit::setText(QString text)
{
	mLineEdit->setText(text);
}

QString ViLineEdit::text()
{
	return mLineEdit->text();
}
