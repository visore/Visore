#include "vimatchwidget.h"
#include "ui_vimatchwidget.h"

ViMatchWidget::ViMatchWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMatchWidget();
	mUi->setupUi(this);
}

ViMatchWidget::~ViMatchWidget()
{
	delete mUi;
}
