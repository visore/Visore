#include <vicoverwidget.h>
#include <QPainter>

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
	repaint();
}

void ViCoverWidget::setImage(QString path)
{
	if(path == "") clear();
	else mImage = QImage(path);
	repaint();
}

void ViCoverWidget::setImage(QImage image)
{
	mImage = image;
	repaint();
}

int ViCoverWidget::calculateBorder()
{
	QString style = styleSheet().toLower();
	int start = style.indexOf("border-radius:");
	if(start >= 0)
	{
		int end = style.indexOf(";", start);
		return style.mid(start, end - start).replace("border-radius:", "").replace("px", "").replace(" ", "").toInt();
	}
	return 0;
}

void ViCoverWidget::paintEvent(QPaintEvent *event)
{
	if(!mImage.isNull())
	{
		int border = calculateBorder();
		QBrush brush(mImage.scaled(width(), height(), Qt::KeepAspectRatioByExpanding));
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setBrush(brush);
		painter.drawRoundedRect(0, 0, width(), height(), border, border);
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
