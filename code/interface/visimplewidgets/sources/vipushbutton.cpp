#include "vipushbutton.h"

ViPushButton::ViPushButton(QWidget *parent)
	: QPushButton(parent)
{
	mIsHover = false;
	mIsPressed = false;
	mHasGlow = false;

	mNormalGradient.setColorAt(0, ViThemeManager::color(4));
	mNormalGradient.setColorAt(0.5, ViThemeManager::color(2));
	mNormalGradient.setColorAt(1, ViThemeManager::color(4));

	mSelectedGradient.setColorAt(0, ViThemeManager::color(13));
	mSelectedGradient.setColorAt(0.5, ViThemeManager::color(11));
	mSelectedGradient.setColorAt(1, ViThemeManager::color(13));

	mDisabledGradient.setColorAt(0, ViThemeManager::color(5));
	mDisabledGradient.setColorAt(0.5, ViThemeManager::color(3));
	mDisabledGradient.setColorAt(1, ViThemeManager::color(5));

	setGlow(ViThemeManager::color(14));
	setCornerRounding(25);
	setSize(64, 64);
	setChecked(false);
}

void ViPushButton::setSize(int width, int height)
{
	setMinimumSize(width, height);
	setMaximumSize(width, height);
}

void ViPushButton::setIcon(ViThemeImage image, ViThemeImage::State state)
{
	if(state == ViThemeImage::Normal)
	{
		mNormalImage = image.image();
	}
	else if(state == ViThemeImage::Selected)
	{
		mSelectedImage = image.image();
	}
	else if(state == ViThemeImage::Disabled)
	{
		mDisbaledImage = image.image();
	}
	else if(state == ViThemeImage::Hovered)
	{
		mHoveredImage = image.image();
	}
	repaint();
}

void ViPushButton::setIcon(ViThemeImage image, ViThemeImage::State state, int width, int height)
{
	QImage scaledImage;
	if(width > 0 && height > 0)
	{
		scaledImage = image.image().scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	else if(width > 0)
	{
		scaledImage = image.image().scaledToWidth(width, Qt::SmoothTransformation);
	}
	else if (height > 0)
	{
		scaledImage = image.image().scaledToHeight(height, Qt::SmoothTransformation);
	}

	if(state == ViThemeImage::Normal)
	{
		mNormalImage = scaledImage;
	}
	else if(state == ViThemeImage::Selected)
	{
		mSelectedImage = scaledImage;
	}
	else if(state == ViThemeImage::Disabled)
	{
		mDisbaledImage = scaledImage;
	}
	else if(state == ViThemeImage::Hovered)
	{
		mHoveredImage = scaledImage;
	}
	repaint();
}

void ViPushButton::setGlow(QColor color, ViGradientCreator::ViGradientType type, int x, int y, int width, int height)
{
	mHasGlow = true;
	mGlowColor = color;
	mGlowType = type;
	mGlowX = x;
	mGlowY = y;
	mGlowWidth = width;
	mGlowHeight = height;
}

void ViPushButton::setCornerRounding(qreal value)
{
	mCornerRounding = value;
}

void ViPushButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	int halfHeight = height() / 2;
	int halfWidth = width() / 2;

	static const int glowOffset = 4;
	static const int glowSizeOffset = glowOffset * 2;

	if(mIsHover && mHasGlow && isEnabled())
	{
		int glowX = mGlowX;
		int glowY = mGlowY;
		int glowWidth = mGlowWidth;
		int glowHeight = mGlowHeight;
		if(glowX < 0)
		{
			glowX = rect().x();
		}
		if(glowY < 0)
		{
			glowY = rect().y();
		}
		if(glowWidth < 0)
		{
			glowWidth = width();
		}
		if(glowHeight < 0)
		{
			glowHeight = height();
		}
		QImage gradient = ViGradientCreator::createGradient(mGlowType, glowWidth, glowHeight, mGlowColor);
		painter.drawImage(rect(), gradient, gradient.rect());
	}
	
	QRect rectangle(glowOffset, glowOffset, width() - glowSizeOffset, height() - glowSizeOffset);	

	if(!isEnabled())
	{
		mDisabledGradient.setStart(halfWidth, 0);
		mDisabledGradient.setFinalStop(halfWidth, height());
		painter.setBrush(mDisabledGradient);
	}
	else if(isCheckable() && isChecked())
	{
		mSelectedGradient.setStart(halfWidth, 0);
		mSelectedGradient.setFinalStop(halfWidth, height());
		painter.setBrush(mSelectedGradient);
	}
	else if(mIsPressed)
	{
		int newWidth = rectangle.width() * 0.98;
		int newHeight = rectangle.height() * 0.98;
		rectangle = QRect(rectangle.x() + ((rectangle.width() - newWidth) / 2), rectangle.y() + ((rectangle.height() - newHeight) / 2), newWidth, newHeight);
		mSelectedGradient.setFinalStop(halfWidth, height());
		painter.setBrush(mSelectedGradient);
	}
	else
	{
		mNormalGradient.setStart(halfWidth, 0);
		mNormalGradient.setFinalStop(halfWidth, height());
		painter.setBrush(mNormalGradient);
	}

	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(rectangle, mCornerRounding, mCornerRounding, Qt::RelativeSize);

	rectangle = QRect((width() - mNormalImage.width()) / 2, (height() - mNormalImage.height()) / 2, mNormalImage.width(), mNormalImage.height());
	painter.drawImage(rectangle, mNormalImage, mNormalImage.rect());
}

void ViPushButton::setEnabled(bool enable)
{
	QPushButton::setEnabled(enable);
	repaint();
}

void ViPushButton::enable()
{
	setEnabled(true);
}

void ViPushButton::disable()
{
	setEnabled(false);
}

void ViPushButton::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViPushButton::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViPushButton::mouseReleaseEvent(QMouseEvent *event)
{
	if(isCheckable())
	{
		setChecked(!isChecked());
		emit clicked(isChecked());
	}
	else
	{
		emit clicked();
	}
	mIsPressed = false;
	repaint();
}

void ViPushButton::mousePressEvent(QMouseEvent *event)
{
	mIsPressed = true;
	repaint();
}
