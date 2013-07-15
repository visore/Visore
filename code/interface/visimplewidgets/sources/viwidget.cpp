#include "viwidget.h"
#include <QPainter>
#include <QMetaMethod>
#include <QStyleOption>

ViWidget::ViWidget(QWidget *parent)
    : QWidget(parent), ViId()
{
	mParent = parent;
    mEngine = ViAudioEngine::instance();
	mStyleSheet = "";
    setObjectName("viwidget" + id());
}

ViWidget::~ViWidget()
{
}

void ViWidget::addStyleSheet(QString style)
{
	setStyleSheet(styleSheet() + style);
}

ViAudioEnginePointer ViWidget::engine()
{
    return mEngine;
}

void ViWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	QStyleOption option;
	option.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

bool ViWidget::addFunctionCall(QString signal, ViFunctionCall functionCall)
{
	QString newSignal = signal;
	while(newSignal.at(0).isDigit())
	{
		newSignal.remove(0, 1);
	}

	if(metaObject()->indexOfSignal(newSignal.toLatin1().data()) == -1)
	{
		LOG("The widget doesn't have a signal \"" + QString(newSignal) + "\"");
		return false;
	}
    mFunctionCalls[newSignal.replace("()", "")] = ViFunctionCall(functionCall);
	QObject::connect(this, signal.toLatin1().data(), this, SLOT(executeFunction()));
	return true;
}

bool ViWidget::executeFunction()
{
	QString name = QString(metaObject()->method(senderSignalIndex()).name());
    mFunctionCalls[name].execute();
}

void ViWidget::setSize(int widthHeight)
{
	setWidth(widthHeight);
	setHeight(widthHeight);
}

void ViWidget::setSize(int width, int height)
{
	setWidth(width);
	setHeight(height);
}

void ViWidget::setSize(QSize size)
{
	setSize(size.width(), size.height());
}

void ViWidget::setWidth(int width)
{
	setMinimumWidth(width);
	setMaximumWidth(width);
}

void ViWidget::setHeight(int height)
{
	setMinimumHeight(height);
	setMaximumHeight(height);
}
