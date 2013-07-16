#include <vistackedwidget.h>
#include <viscrollarea.h>
#include <vilogger.h>

ViStackedWidget::ViStackedWidget()
{
	mWidget = new QStackedWidget();
    mTemporaryIndex = -1;
}

ViStackedWidget::~ViStackedWidget()
{
    //The ownership of mWidget is transfered to ViMainWindow which will delete it.
}

void ViStackedWidget::changeCurrentIndex()
{
	setCurrentIndex(sender()->property("index").toInt());
}

void ViStackedWidget::deleteTemporaryWidget()
{
    if(mTemporaryIndex >= 0)
    {
        QWidget *widget = mWidget->widget(mTemporaryIndex);
        mWidget->removeWidget(widget);
        delete widget;
        mTemporaryIndex = -1;
        QObject::disconnect(mWidget, SIGNAL(currentChanged(int)), this, SLOT(deleteTemporaryWidget()));
    }
}

void ViStackedWidget::showTemporaryWidget(ViWidget *widget, bool scroll)
{
    if(scroll)
    {
        ViScrollArea *scrollArea = new ViScrollArea(ViStackedWidget::widget());
        scrollArea->setWidget(widget);
        instance()->mTemporaryIndex = ViStackedWidget::widget()->addWidget(scrollArea);
    }
    else
    {
        instance()->mTemporaryIndex = ViStackedWidget::widget()->addWidget(widget);
    }
    ViStackedWidget::widget()->setCurrentIndex(instance()->mTemporaryIndex);
    QObject::connect(ViStackedWidget::widget(), SIGNAL(currentChanged(int)), instance().data(), SLOT(deleteTemporaryWidget()));
}

int ViStackedWidget::addWidget(ViWidget *widget, bool scroll)
{
    if(scroll)
	{
        ViScrollArea *scrollArea = new ViScrollArea(ViStackedWidget::widget());
        scrollArea->setWidget(widget);
        return ViStackedWidget::widget()->addWidget(scrollArea);
	}
	else
	{
		return ViStackedWidget::widget()->addWidget(widget);
	}
}

QStackedWidget* ViStackedWidget::widget()
{
	return instance()->mWidget;
}

ViWidget* ViStackedWidget::widget(QString widgetName)
{
	widgetName = widgetName.toLower();
	if(widgetName.startsWith("vi"))
	{
		widgetName = widgetName.remove(0, 2);
	}
	QStackedWidget *stack = ViStackedWidget::widget();
	ViWidget *widget;
	for(int i = 0; i < stack->count(); ++i)
	{
		widget = dynamic_cast<ViWidget*>(stack->widget(i));
		STATICLOG(widget->name().toLower()+"  "+widgetName);
		if(widget != NULL && widget->name().toLower() == widgetName)
		{
			return widget;
		}
	}
	STATICLOG("The specified widget does not exist or is not a subclass of ViWidget.", QtCriticalMsg, "ViStackedWidget");
	return NULL;
}

void ViStackedWidget::setCurrentWidget(ViWidget *widget)
{
	if(widget != NULL)
	{
		ViStackedWidget::widget()->setCurrentWidget(widget);
	}
}

void ViStackedWidget::setCurrentWidget(QString widgetName)
{
	setCurrentWidget(widget(widgetName));
}

void ViStackedWidget::setCurrentIndex(int index)
{
	ViStackedWidget::widget()->setCurrentIndex(index);
}

ViFunctionCall ViStackedWidget::currentIndexCall(int index)
{
	return ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index);
}
