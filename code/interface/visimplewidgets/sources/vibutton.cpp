#include <vibutton.h>
#include <vithememanager.h>

ViButton::ViButton(QWidget *parent)
	: QToolButton(parent)
{
	setCursor(Qt::PointingHandCursor);
	setToolButtonStyle(Qt::ToolButtonIconOnly);

	mTextColor = ViThemeManager::color(ViThemeColors::ButtonTextColor2);
	mEnableBorder = true;
	mEnableBackground = true;
	initialize();
}

void ViButton::clearStyleSheet()
{
	setStyleSheet("");
}

void ViButton::addStyleSheet(QString style)
{
	setStyleSheet(styleSheet() + style);
}

void ViButton::addStyleSheet(QString style, ViButton::Mode mode)
{
	if(mode == ViButton::Normal)
	{
		addStyleSheet("QToolButton{" + style + "}");
	}
	else if(mode == ViButton::Hovered)
	{
		addStyleSheet("QToolButton:hover{" + style + "}");
	}
	else if(mode == ViButton::Selected)
	{
		addStyleSheet("QToolButton:pressed{" + style + "}");
	}
}

void ViButton::initialize()
{
	clearStyleSheet();
	addStyleSheet("color: " + mTextColor.name() + ";", ViButton::Normal);

	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	if(text() == "")
	{
		setToolButtonStyle(Qt::ToolButtonIconOnly);
	}

	if(mEnableBorder)
	{
		QString borderColor1 = ViThemeManager::color(ViThemeColors::BorderColor1).name();
		QString borderColor2 = ViThemeManager::color(ViThemeColors::BorderColor2).name();
		addStyleSheet("border: 2px solid  " + borderColor2 + "; border-radius: 10px;", ViButton::Normal);
		addStyleSheet("border-color: " + borderColor1 + ";", ViButton::Hovered);
	}
	else
	{
		addStyleSheet("border: 0px;", ViButton::Normal);
	}

	if(mEnableBorder)
	{
		QString backgroundColor1 = ViThemeManager::color(ViThemeColors::ButtonNormalColor1).name();
		QString backgroundColor2 = ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name();
		addStyleSheet("padding: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.05 " + backgroundColor1 + ", stop: 0.5 " + backgroundColor2 + ", stop: 0.95 " + backgroundColor1 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Normal);
		addStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Hovered);
	}
	else
	{
		addStyleSheet("background: transparent;", ViButton::Normal);
	}
}

void ViButton::disable()
{
	setEnabled(false);
}

void ViButton::enable()
{
	setEnabled(true);
}

void ViButton::enableBorder()
{
	mEnableBorder = true;
	initialize();
}

void ViButton::disbaleBorder()
{
	mEnableBorder = false;
	initialize();
}

void ViButton::enableBackground()
{
	mEnableBackground = true;
	initialize();
}

void ViButton::disableBackground()
{
	mEnableBackground = false;
	initialize();
}

void ViButton::setText(const QString &text)
{
	QToolButton::setText(text);
}

void ViButton::setText(const QString &text, const QColor &color, const QFont &font)
{
	setText(text);
	setFont(color, font);
}

void ViButton::setText(const QString &text, const ViFont &font)
{
	setText(text);
	setFont(font);
}

void ViButton::setFont(const QColor &color, const QFont &font)
{
	mTextColor = color;
	QToolButton::setFont(font);
	initialize();
}

void ViButton::setFont(const ViFont &font)
{
	setFont(font.color(), font);
}

void ViButton::setSize(const int &width, const int &height)
{
	setWidth(width);
	setHeight(height);
}

void ViButton::setWidth(const int &width)
{
	setMinimumWidth(width);
	setMaximumWidth(width);
}

void ViButton::setHeight(const int &height)
{
	setMinimumHeight(height);
	setMaximumHeight(height);
}

void ViButton::setIcon(const ViThemeIcon &icon, const int &size)
{
	setIcon(icon.icon(ViThemeIcon::Normal, size), size);
}

void ViButton::setIcon(const QIcon &icon, const int &size)
{
	QToolButton::setIcon(icon);
	setIconSize(QSize(size, size));
}
