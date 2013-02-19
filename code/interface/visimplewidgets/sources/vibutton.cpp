#include <vibutton.h>
#include <QStyleOptionButton>

#define GLOW_OFFSET_PERCENTAGE 0.07

ViButton::ViButton(QWidget *parent)
	: ViWidget(parent)
{
	mBackgroundEnabled = true;

	mIsHover = false;
	mIsPressed = false;
	mGlowEnabled = false;

	mIsEnabled = true;
	mIsCheckable = false;
	mIsChecked = false;

	mIconSize = -1;

	mText = "";

	mNormalGradient.setColorAt(0, ViThemeManager::color(ViThemeColors::ButtonNormalColor1));
	mNormalGradient.setColorAt(0.5, ViThemeManager::color(ViThemeColors::ButtonNormalColor2));
	mNormalGradient.setColorAt(1, ViThemeManager::color(ViThemeColors::ButtonNormalColor1));

	mSelectedGradient.setColorAt(0, ViThemeManager::color(ViThemeColors::ButtonSelectedColor1));
	mSelectedGradient.setColorAt(0.5, ViThemeManager::color(ViThemeColors::ButtonSelectedColor2));
	mSelectedGradient.setColorAt(1, ViThemeManager::color(ViThemeColors::ButtonSelectedColor1));

	mDisabledGradient.setColorAt(0, ViThemeManager::color(ViThemeColors::ButtonHoveredColor1));
	mDisabledGradient.setColorAt(0.5, ViThemeManager::color(ViThemeColors::ButtonHoveredColor2));
	mDisabledGradient.setColorAt(1, ViThemeManager::color(ViThemeColors::ButtonHoveredColor1));

	setGlow(ViThemeManager::color(ViThemeColors::ButtonGlowColor1));
	setSize(64, 64);
	setCornerRounding(15);
}

void ViButton::setIcon(QString icon, int size)
{
	setIcon(ViThemeManager::icon(icon), size);
}

void ViButton::setIcon(ViThemeIcon icon, int size)
{
	mIcon = icon;
	mIconSize = size;
	repaint();
}

void ViButton::setText(QString text)
{
	QFont font;
	font.setPointSize(15);
	setText(text, ViThemeManager::color(ViThemeColors::ButtonTextColor1), font);
}

void ViButton::setText(QString text, ViFont font)
{
	mText = text;
	mTextColor = font.color();
	mTextFont = font;
}

void ViButton::setText(QString text, QColor color, QFont font)
{
	mText = text;
	mTextColor = color;
	mTextFont = font;
}

void ViButton::setCornerRounding(qreal value)
{
	mCornerRounding = value;
}

void ViButton::paintEvent(QPaintEvent *event)
{
	ViWidget::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	//Used to draw style sheets with ViButton:pressed
	QStyleOptionButton option;
	option.initFrom(this);
	if(mIsPressed)
	{
		option.state |= QStyle::State_Sunken;
		style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
	}

	paintGlow(painter);
	paintBackground(painter);
	paintIcon(painter);
	paintText(painter);
}

void ViButton::paintGlow(QPainter &painter)
{
	painter.setPen(Qt::NoPen);
	if(mIsHover && mGlowEnabled && isEnabled())
	{
		QImage gradient = ViGradientCreator::createGradient(mGlowType, width(), height(), mGlowColor);
		painter.drawImage(rect(), gradient, gradient.rect());
	}
}

void ViButton::paintBackground(QPainter &painter)
{
	if(mBackgroundEnabled)
	{
		painter.setPen(Qt::NoPen);
		QRect rectangle = QRect(mGlowOffset, mGlowOffset, width() - (mGlowOffset * 2), height() - (mGlowOffset * 2));

		if(!mIsEnabled)
		{
			painter.setBrush(mDisabledGradient);
		}
		else if((mIsCheckable && mIsChecked) || mIsPressed)
		{
			painter.setBrush(mSelectedGradient);
		}
		else
		{
			painter.setBrush(mNormalGradient);
		}

		if(width() > height())
		{
			painter.drawRoundedRect(rectangle, mCornerRounding, mCornerRounding * (width() / height()), Qt::RelativeSize);
		}
		else if(width() < height())
		{
			painter.drawRoundedRect(rectangle, mCornerRounding * (width() / height()), mCornerRounding, Qt::RelativeSize);
		}
		else
		{
			painter.drawRoundedRect(rectangle, mCornerRounding * 1.5, mCornerRounding * 1.5, Qt::RelativeSize);
		}
	}
}

void ViButton::paintIcon(QPainter &painter)
{
	QRect rectangle;
	QImage image;

	if(!mIsEnabled)
	{
		image = mIcon.image(ViThemeIcon::Disabled, mIconSize);
	}
	else if((mIsCheckable && mIsChecked))
	{
		image = mIcon.image(ViThemeIcon::Selected, mIconSize);
	}
	else if(mIsPressed)
	{
		image = mIcon.image(ViThemeIcon::Selected, mIconSize * 0.95);
	}
	else if(mIsHover)
	{
		image = mIcon.image(ViThemeIcon::Hovered, mIconSize);
	}
	else
	{
		image = mIcon.image(ViThemeIcon::Normal, mIconSize);
	}
	mIconWidth = image.width();
	mIconHeight = image.height();

	if(mText == "")
	{
		rectangle = QRect((width() - mIconWidth) / 2, (height() - mIconHeight) / 2, mIconWidth, mIconHeight);
	}
	else
	{
		rectangle = QRect((width() * GLOW_OFFSET_PERCENTAGE), (height() - mIconHeight + (height() * GLOW_OFFSET_PERCENTAGE)) / 2, mIconWidth, mIconHeight);
	}
	painter.drawImage(rectangle, image, image.rect());
}

void ViButton::paintText(QPainter &painter)
{
	QRect rectangle;
	if(mText != "")
	{
		QPen pen(mTextColor);
		pen.setCapStyle(Qt::RoundCap);
		painter.setPen(pen);
		painter.setFont(mTextFont);
		int offset = (width() * GLOW_OFFSET_PERCENTAGE * 1.5) + mIconWidth;
		painter.drawText(QRect(offset, 0, width() - offset, height()), Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, mText);
	}
}

void ViButton::enableBackground(bool enable)
{
	mBackgroundEnabled = enable;
}

void ViButton::disableBackground()
{
	mBackgroundEnabled = false;
}

void ViButton::setGlow(QColor color, ViGradientCreator::ViGradientType type)
{
	enableGlow(true);
	mGlowColor = color;
	mGlowType = type;
}

void ViButton::setGlow(ViGradientCreator::ViGradientType type)
{
	enableGlow(true);
	mGlowType = type;
}

void ViButton::enableGlow(bool enable)
{
	mGlowEnabled = enable;
}

void ViButton::disableGlow()
{
	mGlowEnabled = false;
}

void ViButton::setEnabled(bool enable)
{
	mIsEnabled = enable;
	repaint();
}

void ViButton::enable()
{
	mIsEnabled = true;
}

void ViButton::disable()
{
	mIsEnabled = false;
}

void ViButton::setCheckable(bool checkable)
{
	mIsCheckable = checkable;
}

bool ViButton::isCheckable()
{
	return mIsCheckable;
}

void ViButton::setChecked(bool checked)
{
	mIsChecked = checked;
}

void ViButton::check()
{
	mIsChecked = true;
}

void ViButton::uncheck()
{
	mIsChecked = false;
}

bool ViButton::isChecked()
{
	return mIsChecked;
}

bool ViButton::pressed()
{
	return mIsPressed;
}

void ViButton::enterEvent(QEvent *event)
{
	mIsHover = true;
	repaint();
}

void ViButton::leaveEvent(QEvent *event)
{
	mIsHover = false;
	repaint();
}

void ViButton::mouseReleaseEvent(QMouseEvent *event)
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

void ViButton::mousePressEvent(QMouseEvent *event)
{
	mIsPressed = true;
	repaint();
}

void ViButton::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = width() / 2;
	mHalfHeight = height() / 2;

	if(height() < width())
	{
		mGlowOffset = height() * GLOW_OFFSET_PERCENTAGE;
	}
	else
	{
		mGlowOffset = width() * GLOW_OFFSET_PERCENTAGE;
	}
	++mGlowOffset;

	mNormalGradient.setStart(mHalfWidth, 0);
	mNormalGradient.setFinalStop(mHalfWidth, height());

	mSelectedGradient.setStart(mHalfWidth, 0);
	mSelectedGradient.setFinalStop(mHalfWidth, height());

	mDisabledGradient.setStart(mHalfWidth, 0);
	mDisabledGradient.setFinalStop(mHalfWidth, height());
}
