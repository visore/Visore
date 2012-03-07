#include "vitabbutton.h"

ViTabButton::ViTabButton(QString title, qint8 id, QWidget *parent)
	: QWidget(parent)
{
	mTitle = title;
	mIsHover = false;
	mIsSelected = false;
	mId = id;
	setSize(TAB_WIDTH, TAB_HEIGHT);
}

void ViTabButton::select(bool select)
{
	mIsSelected = select;
	repaint();
}

void ViTabButton::setSize(int width, int height)
{
	setMinimumWidth(width);
	setMaximumWidth(width);
	setMinimumHeight(height);
	setMaximumHeight(height);
}

qint8 ViTabButton::id()
{
	return mId;
}

void ViTabButton::paintEvent(QPaintEvent *event)
{
	QLinearGradient gradient(width() / 2, 0, width() / 2, height());
	QPainter painter(this);
	painter.setPen(Qt::NoPen);

	int buttonX = 0;
	int buttonY = TAB_OFFSET;
	int buttonWidth = width() - 0;
	int buttonHeight = height() - TAB_OFFSET;
	static int angle = 13;

	if(mIsHover)
	{
		QImage glow = ViGradientCreator::createGradient(ViGradientCreator::Rectangle, TAB_WIDTH, TAB_WIDTH, ViThemeManager::color(14));
		painter.drawImage(QRect(0, 0, glow.width(), glow.height()), glow, glow.rect());
	}

	if(mIsSelected)
	{
		gradient.setColorAt(0, ViThemeManager::color(14));
		gradient.setColorAt(0.6, ViThemeManager::color(2));
		gradient.setColorAt(1, ViThemeManager::color(2));
	}
	else
	{
		gradient.setColorAt(0, ViThemeManager::color(4));
		gradient.setColorAt(0.6, ViThemeManager::color(1));
		gradient.setColorAt(1, ViThemeManager::color(1));
	}
	painter.setBrush(gradient);

	painter.drawRoundedRect(buttonX, buttonY, buttonWidth, buttonHeight, angle, angle);
	painter.drawRect(buttonX, angle + TAB_OFFSET, buttonWidth, buttonHeight - angle);

	painter.setPen(Qt::white);
	painter.drawText(rect(), Qt::AlignCenter, mTitle);
}

void ViTabButton::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViTabButton::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViTabButton::mousePressEvent(QMouseEvent *event)
{
	mIsSelected = true;
	repaint();
	emit selected(mId);
}
