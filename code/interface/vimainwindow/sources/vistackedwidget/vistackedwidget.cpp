#include <vistackedwidget.h>
#include <viscrollarea.h>

ViStackedWidget::ViStackedWidget()
{
	mWidget = new QStackedWidget();
}

ViStackedWidget::~ViStackedWidget()
{
    //The ownership of mWidget is transfered to ViMainWindow which will delete it.
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

void ViStackedWidget::setCurrentWidget(ViWidget *widget)
{
	ViStackedWidget::widget()->setCurrentWidget(widget);
}

void ViStackedWidget::setCurrentIndex(int index)
{
	ViStackedWidget::widget()->setCurrentIndex(index);
}

ViFunctionCall ViStackedWidget::currentIndexCall(int index)
{
	return ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index);
}
