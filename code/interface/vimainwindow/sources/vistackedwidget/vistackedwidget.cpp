#include <vistackedwidget.h>
#include <viscrollarea.h>

ViStackedWidget::ViStackedWidget()
{
	mWidget = new QStackedWidget();
}

ViStackedWidget::~ViStackedWidget()
{
	delete mWidget;
}

int ViStackedWidget::addWidget(ViWidget *widget, bool scroll)
{
	if(scroll)
	{
		ViScrollArea *scroll = new ViScrollArea(ViStackedWidget::widget());
		scroll->setWidget(widget);
		return ViStackedWidget::widget()->addWidget(scroll);
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

void ViStackedWidget::setCurrentWidget(ViWidget *widget)
{
	ViStackedWidget::widget()->setCurrentWidget(widget);
}

void ViStackedWidget::setCurrentIndex(int index)
{STATICLOG("+++: "+QString::number(index));
	ViStackedWidget::widget()->setCurrentIndex(index);
}

ViFunctionCall ViStackedWidget::currentIndexCall(int index)
{STATICLOG("++----+: "+QString::number(index));
	return ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index);
}
