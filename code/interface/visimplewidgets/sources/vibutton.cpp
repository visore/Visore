#include <vibutton.h>
#include <vithememanager.h>
#include <QEvent>

ViButton::ViButton(QWidget *parent)
	: QToolButton(parent)
{
	QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(changeIcon()));

	//setCursor(Qt::PointingHandCursor);
	setToolButtonStyle(Qt::ToolButtonIconOnly);

	mSelectionEnabled = false;
	mIsSelected = false;

	mEnableBorder = true;
	mEnableBackground = true;

	ViFont font;
	font.setFamily("Harabara");
	font.setPointSize(16);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	font.setColor(ViThemeManager::color(ViThemeColors::ButtonTextColor1));
	setFont(font);

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
	else if(mode == ViButton::Checked)
	{
		addStyleSheet("QToolButton:checked{" + style + "}");
	}
	else if(mode == ViButton::Enabled)
	{
		addStyleSheet("QToolButton:enabled{" + style + "}");
	}
	else if(mode == ViButton::Disabled)
	{
		addStyleSheet("QToolButton:disabled{" + style + "}");
	}
}

void ViButton::changeIcon(const bool &hover)
{
	if(!isEnabled())
	{
		QToolButton::setIcon(mIcon.icon(ViThemeIcon::Disabled));
	}
	else if(hover)
	{
		if(isCheckable())
		{
			if(isChecked())
			{
				QToolButton::setIcon(mIcon.icon(ViThemeIcon::Normal));
			}
			else
			{
				QToolButton::setIcon(mIcon.icon(ViThemeIcon::Selected));
			}
		}
		else
		{
			QToolButton::setIcon(mIcon.icon(ViThemeIcon::Hovered));
		}
	}
	else if(isCheckable() && isChecked())
	{
		QToolButton::setIcon(mIcon.icon(ViThemeIcon::Selected));
	}
	else
	{
		QToolButton::setIcon(mIcon.icon(ViThemeIcon::Normal));
	}
}

void ViButton::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::EnabledChange)
	{
		changeIcon();
	}
}

void ViButton::enterEvent(QEvent*)
{
	changeIcon(true);
}

void ViButton::leaveEvent(QEvent*)
{
	changeIcon();
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
		addStyleSheet("border: 2px solid  " + ViThemeManager::color(ViThemeColors::BorderColor2).name() + "; border-radius: 10px;", ViButton::Normal);
		addStyleSheet("border-color: " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + ";", ViButton::Hovered);
		addStyleSheet("border-color: " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + ";", ViButton::Checked);
		addStyleSheet("border-color: " + ViThemeManager::color(ViThemeColors::BorderColor3).name() + ";", ViButton::Disabled);
	}
	else
	{
		addStyleSheet("border: 0px;", ViButton::Normal);
	}

	if(mEnableBackground)
	{
		QString backgroundColor1 = ViThemeManager::color(ViThemeColors::ButtonNormalColor1).name();
		QString backgroundColor2 = ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name();
		addStyleSheet("padding: 5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.05 " + backgroundColor1 + ", stop: 0.5 " + backgroundColor2 + ", stop: 0.95 " + backgroundColor1 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Normal);

		backgroundColor1 = ViThemeManager::color(ViThemeColors::ButtonHoveredColor1).name();
		backgroundColor2 = ViThemeManager::color(ViThemeColors::ButtonHoveredColor2).name();
		addStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Hovered);
		addStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Checked);

		backgroundColor1 = ViThemeManager::color(ViThemeColors::ButtonDisbaledColor1).name();
		backgroundColor2 = ViThemeManager::color(ViThemeColors::ButtonDisbaledColor2).name();
		addStyleSheet("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");", ViButton::Disabled);
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

void ViButton::disableBorder()
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
	initialize();
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

void ViButton::setFont(const QFont &font)
{
	QToolButton::setFont(font);
}

void ViButton::setFont(const ViFont &font)
{
	setFont(font.color(), font);
}

void ViButton::setSize(const int &widthHeight)
{
	setWidth(widthHeight);
	setHeight(widthHeight);
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
	mIcon = icon;
	setIconSize(QSize(size, size));
	changeIcon();
}
