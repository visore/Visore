#include <vistackedconnector.h>
#include <vistackedwidget.h>

ViStackedConnector::ViStackedConnector()
{
	mIndex = -1;
}

void ViStackedConnector::setConnection(ViWidget *widget, char* signal)
{
	if(mIndex >= 0)
	{
		QObject::disconnect(0, 0, this, SLOT(access()));
	}
	mIndex = ViStackedWidget::addWidget(widget);
	QObject::connect(this, signal, this, SLOT(access()));
}

void ViStackedConnector::access()
{
	ViStackedWidget::setCurrentIndex(mIndex);
}
