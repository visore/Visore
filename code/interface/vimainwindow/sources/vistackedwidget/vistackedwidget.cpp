#include <vistackedwidget.h>
#include <viscrollarea.h>

ViStackedWidget::ViStackedWidget()
{
	mWidget = new QStackedWidget();
    mTemporaryIndex = -1;
}

ViStackedWidget::~ViStackedWidget()
{
    //The ownership of mWidget is transfered to ViMainWindow which will delete it.
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
