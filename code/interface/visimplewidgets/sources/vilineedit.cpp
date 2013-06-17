#include "vilineedit.h"
#include "vithememanager.h"
#include <QEvent>

ViLineEdit::ViLineEdit(QWidget *parent)
	: ViWidget(parent)
{
	mLayout.setContentsMargins(0, 0, 0, 0);
	setLayout(&mLayout);

	mLineEdit = new QLineEdit(this);
	mLayout.addWidget(mLineEdit);
    mLineEdit->installEventFilter(this);

	setStyleSheet("\
		QLineEdit\
		{\
			border-radius: 3px;\
			border: 2px solid " + ViThemeManager::color(ViThemeColors::BorderColor3).name() + ";\
		}\
		QLineEdit:focus\
		{\
			border: 2px solid " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + ";\
		}\
	");

	setHeight(28);

	QObject::connect(mLineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(textChanged(const QString&)));
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

void ViLineEdit::clear()
{
	mLineEdit->clear();
}

void ViLineEdit::setSize(int width, int height)
{
	setWidth(width);
	setHeight(height);
}

void ViLineEdit::setSize(QSize size)
{
	setSize(size.width(), size.height());
}

void ViLineEdit::setWidth(int width)
{
	ViWidget::setWidth(width);
	mLineEdit->setMinimumWidth(width);
	mLineEdit->setMaximumWidth(width);
}

void ViLineEdit::setHeight(int height)
{
	ViWidget::setHeight(height);
	mLineEdit->setMinimumHeight(height);
	mLineEdit->setMaximumHeight(height);
}

bool ViLineEdit::eventFilter(QObject *object, QEvent *event)
{
    if(object == mLineEdit)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            emit doubleClicked();
        }
    }
}
