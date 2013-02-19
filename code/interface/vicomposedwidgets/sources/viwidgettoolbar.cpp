#include <viwidgettoolbar.h>

ViWidgetToolbar::ViWidgetToolbar(Qt::Alignment align, QWidget *parent)
	: ViWidget(parent)
{
	mCentralWidget = new QWidget(this);
	mAlign = align;
	if(mAlign & Qt::AlignTop || mAlign & Qt::AlignBottom)
	{
		mLayout = new QHBoxLayout(mCentralWidget);
	}
	else if(mAlign & Qt::AlignRight || mAlign & Qt::AlignLeft || mAlign & Qt::AlignCenter)
	{
		mLayout = new QVBoxLayout(mCentralWidget);
	}

	QColor color1 = ViThemeManager::color(ViThemeColors::BorderColor3);
	QColor color2 = ViThemeManager::color(ViThemeColors::MainColor3);

	setObjectName("toolbar");
	setStyleSheet
	("\
		QWidget#toolbar{\
			border-radius: 10px;\
			border-style: solid;\
			border-color: " + color1.name() + ";\
			border-width: 3px;\
			background: rgba(" + QString::number(color1.red()) + ", " + QString::number(color1.green()) + ", " + QString::number(color1.blue()) + ", 175);\
		}\
		ViButton{\
			border-radius: 7px;\
			border-style: solid;\
			border-width: 2px;\
			border-color: transparent;\
			padding: 5px;\
			background: transparent;\
		}\
		ViButton:hover{\
			border-color: " + color2.name() + ";\
		}\
		ViButton:pressed{\
			background: rgba(" + QString::number(color2.red()) + ", " + QString::number(color2.green()) + ", " + QString::number(color2.blue()) + ", 80);\
		}\
	");
	mLayout->setContentsMargins(5, 5, 5, 5);
	hide();
}

ViWidgetToolbar::~ViWidgetToolbar()
{
	delete mLayout;
	delete mCentralWidget;
}

void ViWidgetToolbar::refresh()
{
	static qint8 offset = 5;
	mCentralWidget->adjustSize();
	adjustSize();

	if(mAlign & Qt::AlignLeft)
	{
		move(offset, pos().y());
	}
	else if(mAlign & Qt::AlignRight)
	{
		move(mParent->width() - width() - offset, pos().y());
	}
	else if(mAlign & Qt::AlignCenter)
	{
		move((mParent->width() / 2) - (width() / 2), pos().y());
	}

	if(mAlign & Qt::AlignTop)
	{
		move(pos().x(), offset);
	}
	else if(mAlign & Qt::AlignBottom)
	{
		move(pos().x(), mParent->height() - height() - offset);
	}
	else if(mAlign & Qt::AlignCenter)
	{
		move(pos().x(), (mParent->height() / 2) - (height() / 2));
	}
}

void ViWidgetToolbar::addWidget(QWidget *widget)
{
	mLayout->addWidget(widget);
	refresh();
}

void ViWidgetToolbar::show()
{
	refresh();
	ViWidget::show();
}

void ViWidgetToolbar::hide()
{
	refresh();
	ViWidget::hide();
}

void ViWidgetToolbar::setVisible(bool visible)
{
	refresh();
	ViWidget::setVisible(visible);
}
