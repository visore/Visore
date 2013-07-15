#include <vicoverwidget.h>
#include <QPainter>
#include <QStyleOption>

ViCoverWidget::ViCoverWidget(QWidget *parent)
	: ViWidget(parent)
{
	setMouseTracking(true);
	mBorderColor1 = "border-color: " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + "; ";
	mBorderColor2 = "border-color: " + ViThemeManager::color(ViThemeColors::BorderColor2).name() + "; ";
	setStyleSheet(".ViCoverWidget { border-width: 2px; border-style: solid; border-radius: 4px; " + mBorderColor2 + "}");
}

ViCoverWidget::~ViCoverWidget()
{
}

void ViCoverWidget::clear()
{
	mImage = QImage();
}

void ViCoverWidget::setImage(QString path)
{
	if(path == "") clear();
	else mImage = QImage(path);
}

void ViCoverWidget::setImage(QImage image)
{
	mImage = image;
}

void ViCoverWidget::paintEvent(QPaintEvent *event)
{
	if(!mImage.isNull())
	{
		QPainter painter(this);
		painter.drawImage(0, 0, mImage.scaled(width(), height(), Qt::KeepAspectRatioByExpanding));
	}
	ViWidget::paintEvent(event);
}

void ViCoverWidget::enterEvent(QEvent*)
{
	QString style = styleSheet();
	style.replace(mBorderColor2, mBorderColor1);
	setStyleSheet(style);
}

void ViCoverWidget::leaveEvent(QEvent*)
{
	QString style = styleSheet();
	style.replace(mBorderColor1, mBorderColor2);
	setStyleSheet(style);
}
