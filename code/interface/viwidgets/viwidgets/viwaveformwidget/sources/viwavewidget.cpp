#include "viwavewidget.h"

ViWaveWidgetThread::ViWaveWidgetThread(ViWaveWidget *widget)
	: QThread()
{
	mWidget = widget;
	mPosition = -1;
}

void ViWaveWidgetThread::run()
{
	mMutex.lock();
	bool isEmpty = mChunks.isEmpty();
	mMutex.unlock();
	while(!isEmpty)
	{
		mMutex.lock();
		QSharedPointer<ViWaveFormChunk> chunk = mChunks.takeFirst();
		mMutex.unlock();
		qreal *data = chunk->data();

		for(int i = 0; i < chunk->size(); ++i)
		{
			mFormMutex.lock();
			mForm.append(data[i]);
			mFormMutex.unlock();
			/*mFormMutex.lock();
			for(int j = 0; j < mForms.size(); ++j)
			{
				mForms[j].append(data[i]);
			}
			mFormMutex.unlock();*/
		}

		emit tileAvailable();
		mMutex.lock();
		isEmpty = mChunks.isEmpty();
		mMutex.unlock();
	}
}

void ViWaveWidgetThread::analyze(int size)
{
	//mWidget->mEngine->calculateWaveForm(mBufferType, mPosition, size);
}

void ViWaveWidgetThread::changed(QSharedPointer<ViWaveFormChunk> chunk)
{
	mMutex.lock();
	mChunks.append(chunk);
	mMutex.unlock();
	if(!isRunning())
	{
		start();
	}
}

void ViWaveWidgetThread::positionChanged(ViAudioPosition position)
{
	mPosition = position.sample();
	emit tileAvailable();
}

ViWaveWidget::ViWaveWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	setEngine(engine);
	mToolbar = new ViWidgetToolbar(ViWidgetToolbar::Right, parent);
	mToolbar->setEngine(engine);
	mToolbar->addButton("Zoom In", ViThemeManager::icon("zoomin.png"), this, SLOT(zoomIn()));
	mToolbar->addButton("Zoom Out", ViThemeManager::icon("zoomout.png"), this, SLOT(zoomOut()));
	mThread = new ViWaveWidgetThread(this);
	mThread->mBufferType = type;

	if(type == ViAudioBuffer::Original)
	{
		ViObject::connect(mEngine, SIGNAL(originalWaveChanged(QSharedPointer<ViWaveFormChunk>)), mThread, SLOT(changed(QSharedPointer<ViWaveFormChunk>)));
		ViObject::connect(mEngine, SIGNAL(originalBufferChanged(int)), mThread, SLOT(analyze(int)));
	}
	else
	{
		ViObject::connect(mEngine, SIGNAL(correctedWaveChanged(QSharedPointer<ViWaveFormChunk>)), mThread, SLOT(changed(QSharedPointer<ViWaveFormChunk>)));
		ViObject::connect(mEngine, SIGNAL(correctedBufferChanged(int)), mThread, SLOT(analyze(int)));
	}

	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), mThread, SLOT(positionChanged(ViAudioPosition)));
	ViObject::connectQueued(mThread, SIGNAL(tileAvailable()), this, SLOT(repaint()));
	mZoomLevel = 0;
}

ViWaveWidget::~ViWaveWidget()
{
	mThread->quit();
	delete mThread;
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
	int position = mThread->mPosition / (FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, mZoomLevel)); //(FIRST_ZOOM_LEVEL * (mZoomLevel + 1));
	int start = position - halfWidth;
	int end = position + halfWidth;
	if(start < 0)
	{
		start = 0;
	}
	if(end > mThread->mForm.size(mZoomLevel))
	{
		end = mThread->mForm.size(mZoomLevel);
	}
	int drawStart = halfWidth + (start - position);


	if(mThread->mForm.isUnderCutoff(mZoomLevel))
	{
		int previous = halfHeight;
		for(int i = start; i < end; ++i)
		{
			painter.setPen(penNormal);
			mThread->mFormMutex.lock();
			int now = mThread->mForm.maximum(i, mZoomLevel) / ratio;
			painter.drawLine(drawStart, previous, drawStart, now);
			previous = now;
			mThread->mFormMutex.unlock();
			drawStart++;
		}
	}
	else
	{
		for(int i = start; i < end; ++i)
		{
			painter.setPen(penNormal);
			mThread->mFormMutex.lock();
			painter.drawLine(drawStart, halfHeight, drawStart, mThread->mForm.maximum(i, mZoomLevel) / ratio);
			painter.drawLine(drawStart, halfHeight, drawStart, mThread->mForm.minimum(i, mZoomLevel) / ratio);
			mThread->mFormMutex.unlock();

			painter.setPen(penAverage);
			mThread->mFormMutex.lock();
			painter.drawLine(drawStart, halfHeight, drawStart, mThread->mForm.maximumAverage(i, mZoomLevel) / ratio);
			painter.drawLine(drawStart, halfHeight, drawStart, mThread->mForm.minimumAverage(i, mZoomLevel) / ratio);
			mThread->mFormMutex.unlock();
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
