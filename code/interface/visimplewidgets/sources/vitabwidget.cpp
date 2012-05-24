#include "vitabwidget.h"
#include "ui_vitabwidget.h"

ViTabWidget::ViTabWidget(QWidget *parent)
	: QWidget(parent)
{
	mUi = new Ui::ViTabWidget();
	mUi->setupUi(this);
	mCurrentId = 0;
	mMainAngle = 0;
	mButtonAngle = 0;
}

ViTabWidget::~ViTabWidget()
{
	for(int i = 0; i < mButtons.size(); ++i)
	{
		delete mButtons[i];
	}
	mButtons.clear();
	delete mUi;
}

void ViTabWidget::selectTab(qint8 index)
{
	mUi->stackedWidget->setCurrentIndex(index);
}

void ViTabWidget::setSize(int width, int height)
{
	setWidth(width);
	setHeight(height);
}

void ViTabWidget::setWidth(int width)
{
	setMinimumWidth(width);
	setMaximumWidth(width);
}

void ViTabWidget::setHeight(int height)
{
	setMinimumHeight(height);
	setMaximumHeight(height);
}

void ViTabWidget::setRounding(qint8 mainAngle, qint8 buttonAngle)
{
	mMainAngle = mainAngle;
	mButtonAngle = buttonAngle;
	for(int i = 0; i < mButtons.size(); ++i)
	{
		mButtons[i]->setRounding(mButtonAngle);
	}
}

void ViTabWidget::setTabOffset(int offset)
{
	mUi->buttonOffset->changeSize(offset, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void ViTabWidget::tabSelected(qint8 id)
{
	for(int i = 0; i < mButtons.size(); ++i)
	{
		if(mButtons[i]->id() != id)
		{
			mButtons[i]->select(false);
		}
	}
	mUi->stackedWidget->setCurrentIndex(id);
}

void ViTabWidget::addTab(QString title, QWidget *widget)
{
	ViTabButton *button = new ViTabButton(title, mCurrentId, this);
	button->setRounding(mButtonAngle);
	ViObject::connect(button, SIGNAL(selected(qint8)), this, SLOT(tabSelected(qint8)));
	mButtons.append(button);
	mUi->buttonWidget->layout()->addWidget(button);
	mUi->stackedWidget->layout()->addWidget(widget);
	repaint();
	++mCurrentId;
	if(mCurrentId == 1)
	{
		button->select(true);
	}
}

void ViTabWidget::paintEvent(QPaintEvent *event)
{
	static qint16 alpha = 150;
	QLinearGradient gradient(width() / 2, 0, width() / 2, height());
	QColor color = ViThemeManager::color(2);
	color.setAlpha(alpha);
	gradient.setColorAt(0, color);
	gradient.setColorAt(0.8, color);
	color = ViThemeManager::color(3);
	color.setAlpha(alpha);
	gradient.setColorAt(1, color);

	QPainter painter(this);
	painter.setBrush(gradient);
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(mUi->tabContainer->pos().x(), mUi->tabContainer->pos().y(), mUi->tabContainer->width(), mUi->tabContainer->height(), mMainAngle, mMainAngle);
	painter.drawRect(mUi->tabContainer->pos().x(), mUi->tabContainer->pos().y(), mMainAngle, mMainAngle);
}
