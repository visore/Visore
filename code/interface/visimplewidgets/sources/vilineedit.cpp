#include "vilineedit.h"
#include "vithememanager.h"
#include <QEvent>

ViLineEdit::ViLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	installEventFilter(this);
}

bool ViLineEdit::eventFilter(QObject *object, QEvent *event)
{
	if(object == this)
	{
		if(event->type() == QEvent::MouseButtonDblClick)
		{
			emit doubleClicked();
		}
	}
}
