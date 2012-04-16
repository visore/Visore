#include "viwaveformwidget.h"

#define CONTENT_MARGIN -4

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	mWidget = new ViWaveWidget(engine, type, this);
	ViObject::connect(mWidget, SIGNAL(pointerMoved(qint32)), this, SIGNAL(pointerMoved(qint32)));
	ViObject::connect(mWidget, SIGNAL(pointerMoved(qint32)), this, SLOT(setPointer(qint32)));

	parent->setObjectName("parent");
	parent->setStyleSheet("\
		.QWidget#parent{\
			border-radius: 10px;\
			border-style: solid;\
			border-color: rgb(" + QString::number(ViThemeManager::color(14).red()) + ", " + QString::number(ViThemeManager::color(14).green()) + ", " + QString::number(ViThemeManager::color(14).blue()) + ");\
			border-width: 5px;\
		}\
	");
	parent->setContentsMargins(CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);

	mToolbar = new ViWidgetToolbar(ViWidgetToolbar::Right, this);
	mToolbar->setEngine(engine);
	mToolbar->addButton("Zoom In", ViThemeManager::image("zoomin.png", ViThemeImage::Normal, ViThemeManager::Icon).icon(), this, SLOT(zoomIn()));
	mToolbar->addButton("Zoom Out", ViThemeManager::image("zoomout.png", ViThemeImage::Normal, ViThemeManager::Icon).icon(), this, SLOT(zoomOut()));

	mPointerPosition = 0;
	mPointerPosition2 = 0;
	setZoomLevel(6);

}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mToolbar;
	delete mWidget;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	static QPen penPosition(ViThemeManager::color(12));
	static QPen penPointer(ViThemeManager::color(16), 2);

	painter.setPen(penPosition);
	painter.drawLine(0, mHalfHeight, width(), mHalfHeight);

	QRect rectangle(0, 0, mHalfWidth, height());
	QColor color = ViThemeManager::color(11);
	color = QColor(color.red(), color.green(), color.blue(), 100);
	painter.fillRect(rectangle, color);

	painter.drawLine(mHalfWidth, 0, mHalfWidth, height());

	painter.setPen(penPointer);

	int position = mPointerPosition2 / mRatio;
	int maximum = mHalfHeight;
	int minimum = mHalfHeight;
	if(mPointerPosition2 >= 0)
	{
		maximum = mWidget->maximum(position);
		minimum = mWidget->minimum(position);
	}
	painter.drawLine(mPointerPosition + 1, 0, mPointerPosition + 1, maximum - 4);
	painter.drawEllipse(mPointerPosition - 3, maximum - 3, 8, 8);
	if(maximum + 4 < minimum - 4)
	{
		painter.drawLine(mPointerPosition + 1, maximum + 5, mPointerPosition + 1, minimum - 4);
		painter.drawEllipse(mPointerPosition - 3, minimum - 3, 8, 8);
		painter.drawLine(mPointerPosition + 1, minimum + 5, mPointerPosition + 1, height());
	}
	else
	{
		painter.drawLine(mPointerPosition + 1, maximum + 6, mPointerPosition + 1, height());
	}
}

void ViWaveFormWidget::setZoomLevel(qint16 level)
{
	if(level >= 0 && level < MAXIMUM_ZOOM_LEVELS)
	{
		mZoomLevel = level;
	}
	mWidget->setZoomLevel(mZoomLevel);
	mRatio = FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel);
	repaint();
}

void ViWaveFormWidget::setPointer(qint32 position)
{
	mPointerPosition2 = position;
	mPointerPosition = ((position - mWidget->position()) / mRatio) + mHalfWidth;
	update();
}



void ViWaveFormWidget::zoomIn()
{
	setZoomLevel(mZoomLevel - 1);
}

void ViWaveFormWidget::zoomOut()
{
	setZoomLevel(mZoomLevel + 1);
}

void ViWaveFormWidget::resizeEvent(QResizeEvent *event)
{
	mHalfWidth = (width() / 2);
	mHalfHeight = (height() / 2);
	mWidget->resize(event->size());
	mToolbar->refresh();
}

void ViWaveFormWidget::enterEvent(QEvent *event)
{
	mToolbar->show();
}

void ViWaveFormWidget::leaveEvent(QEvent *event)
{
	QPoint mouse = QCursor::pos();
	QPoint position = mapToGlobal(pos());
	if(mouse.x() < position.x() || mouse.x() > position.x() + width() || mouse.y() < position.y() || mouse.y() > position.y() + height())
	{
		mToolbar->hide();
	}
}
