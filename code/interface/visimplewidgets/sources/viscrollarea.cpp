#include <viscrollarea.h>
#include <QPainter>
#include <QStyleOption>

ViScrollArea::ViScrollArea(QWidget *parent)
	: QScrollArea(parent)
{
	/*setStyleSheet("\
		QScrollArea\
		{\
			background: transparent;\
			border: 0px;\
		}\
		QScrollArea > QWidget > QWidget\
		{\
			background: transparent;\
			border: 0px;\
		}\
	");*/

	setWidgetResizable(true);
}

ViScrollArea::~ViScrollArea()
{
}

