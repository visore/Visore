#include "viwavewidget.h"

ViWaveWidget::ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	setEngine(engine);
	mToolbar = new ViWidgetToolbar(ViWidgetToolbar::Right, parent);
	mToolbar->setEngine(engine);
	mToolbar->addButton("Zoom In", ViThemeManager::image("zoomin.png", ViThemeImage::Normal, ViThemeManager::Icon).icon(), this, SLOT(zoomIn()));
	mToolbar->addButton("Zoom Out", ViThemeManager::image("zoomout.png", ViThemeImage::Normal, ViThemeManager::Icon).icon(), this, SLOT(zoomOut()));

	mForm = mEngine->waveSummary(type);
	if(type == ViAudioBuffer::Original)
	{
		ViObject::connect(mEngine, SIGNAL(originalWaveChanged()), this, SLOT(repaint()));
	}
	else
	{
		ViObject::connect(mEngine, SIGNAL(correctedWaveChanged()), this, SLOT(repaint()));
	}
	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(positionChanged(ViAudioPosition)));
	
	mPosition = 0;
	mZoomLevel = 15;
}

ViWaveWidget::~ViWaveWidget()
{
	delete mToolbar;
}

void ViWaveWidget::paintEvent(QPaintEvent *event)
{
	resize(mParent->width(), mParent->height());
	QPainter painter(this);

	static QPen penNormal(ViThemeManager::color(15));
	static QPen penAverage(ViThemeManager::color(14));
	static QPen penPosition(ViThemeManager::color(12));

	qreal ratio = UNSIGNED_CHAR_HALF_VALUE / (height() / 2.0);

	int halfHeight = height() / 2;
	int halfWidth = width() / 2;
	int position = mPosition / (FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel));
	int start = position - halfWidth;
	int end = position + halfWidth;

	int zoomSize = mForm->size(mZoomLevel);
	bool underCutOff = mForm->isUnderCutoff(mZoomLevel);

	if(start < 0)
	{
		start = 0;
	}
	if(end > zoomSize)
	{
		end = zoomSize;
	}
	int drawStart = halfWidth + (start - position);

	if(underCutOff)
	{
		int previous = halfHeight;
		for(int i = start; i < end; ++i)
		{
			painter.setPen(penNormal);
			int now = mForm->maximum(i, mZoomLevel) / ratio;
			painter.drawLine(drawStart, previous, drawStart + 1, now);
			previous = now;
			drawStart++;
		}
	}
	else
	{
		for(int i = start; i < end; ++i)
		{
			painter.setPen(penNormal);
			painter.drawLine(drawStart, halfHeight, drawStart, mForm->maximum(i, mZoomLevel) / ratio);
			painter.drawLine(drawStart, halfHeight, drawStart, mForm->minimum(i, mZoomLevel) / ratio);

			painter.setPen(penAverage);
			painter.drawLine(drawStart, halfHeight, drawStart, mForm->maximumAverage(i, mZoomLevel) / ratio);
			painter.drawLine(drawStart, halfHeight, drawStart, mForm->minimumAverage(i, mZoomLevel) / ratio);
			drawStart++;
		}
	}

	painter.setPen(penPosition);
	painter.drawLine(0, halfHeight, width(), halfHeight);

	QRect rectangle(0, 0, halfWidth, height());
	QColor color = ViThemeManager::color(11);
	color = QColor(color.red(), color.green(), color.blue(), 100);
	painter.fillRect(rectangle, color);

	painter.drawLine(halfWidth, 0, halfWidth, height());
}

void ViWaveWidget::resizeEvent(QResizeEvent *event)
{
	mToolbar->refresh();
}

void ViWaveWidget::enterEvent(QEvent *event)
{
	mToolbar->show();
}

void ViWaveWidget::leaveEvent(QEvent *event)
{
	QPoint mouse = QCursor::pos();
	QPoint position = mapToGlobal(pos());
	if(mouse.x() < position.x() || mouse.x() > position.x() + width() || mouse.y() < position.y() || mouse.y() > position.y() + height())
	{
		mToolbar->hide();
	}
}

void ViWaveWidget::zoomIn()
{
	qint8 level = mZoomLevel - 1;
	if(level >= 0)
	{
		mZoomLevel = level;
	}
	repaint();
}

void ViWaveWidget::zoomOut()
{
	qint8 level = mZoomLevel + 1;
	if(level < MAXIMUM_ZOOM_LEVELS)
	{
		mZoomLevel = level;
	}
	repaint();
}

void ViWaveWidget::positionChanged(ViAudioPosition position)
{
	mPosition = position.sample();
	repaint();
}
