#include "vitabbutton.h"

ViTabButton::ViTabButton(QString title, qint8 id, QWidget *parent)
	: QWidget(parent)
{
	mTitle = title;
	mIsHover = false;
	mIsSelected = false;
	mId = id;
	mAngle = 0;
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

void ViTabButton::setRounding(qint8 angle)
{
	mAngle = angle;
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

	static qint16 alpha = 150;
	static int buttonX = 0;
	static int buttonY = TAB_OFFSET;
	int buttonWidth = width() - 0;
	int buttonHeight = height() - TAB_OFFSET;

	if(mIsHover)
	{
		QImage glow = ViGradientCreator::createGradient(ViGradientCreator::Rectangle, TAB_WIDTH, TAB_WIDTH, ViThemeManager::color(ViThemeColors::MainColor1));
		glow = glow.copy(0, 0, glow.width(), mAngle * 2);
		painter.drawImage(QRect(0, mAngle, glow.width(), glow.height()), glow, glow.rect());
	}

	if(mIsSelected)
	{
		QColor color = ViThemeManager::color(ViThemeColors::MainColor1);
		gradient.setColorAt(0, color);
		color = ViThemeManager::color(ViThemeColors::MainColor2);
		gradient.setColorAt(0.6, color);
		color.setAlpha(alpha);
		gradient.setColorAt(0.7, color);
	}
	else
	{
		QColor color;
		if(mIsHover)
		{
			color = ViThemeManager::color(ViThemeColors::MainColor1);
		}
		else
		{
			color = ViThemeManager::color(ViThemeColors::TextColor1);
		}
		gradient.setColorAt(0, color);
		color = ViThemeManager::color(ViThemeColors::MainColor1);
		gradient.setColorAt(0.6, color);
		color.setAlpha(alpha);
		gradient.setColorAt(1, color);
	}
	painter.setBrush(gradient);
	painter.drawRoundedRect(buttonX, buttonY + mAngle, buttonWidth, buttonHeight, mAngle, mAngle);

	painter.setPen(Qt::white);
	painter.drawText(QRect(rect().x(), rect().y() + mAngle, rect().width(), rect().height()), Qt::AlignCenter, mTitle);
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
