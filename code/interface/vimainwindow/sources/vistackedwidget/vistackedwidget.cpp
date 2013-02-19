#include <vistackedwidget.h>

ViStackedWidget::ViStackedWidget()
{
	mWidget = new QStackedWidget();
}

ViStackedWidget::~ViStackedWidget()
{
	delete mWidget;
}

int ViStackedWidget::addWidget(ViWidget *widget)
{
	return ViStackedWidget::widget()->addWidget(widget);
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

void ViStackedWidget::setCurrentIndex(QVariant index)
{
	ViStackedWidget::setCurrentIndex(index.toInt());
}

ViFunctionCall ViStackedWidget::currentIndexCall(int index)
{
	return ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index);
}
