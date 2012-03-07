#include "viwavewidget.h"

ViWaveWidgetThread::ViWaveWidgetThread(ViWaveWidget *widget)
	: QThread()
{
	mWidget = widget;
	mPosition = -1;
	mCompressionLevels.append(COMPRESSION_LEVEL_1);
	mCompressionLevels.append(COMPRESSION_LEVEL_2);
	mCompressionLevels.append(COMPRESSION_LEVEL_3);
	mCompressionLevels.append(COMPRESSION_LEVEL_4);
	mCompressionLevels.append(COMPRESSION_LEVEL_5);
	mCompressionLevels.append(COMPRESSION_LEVEL_6);
	mCompressionLevels.append(COMPRESSION_LEVEL_7);
	mCompressionLevels.append(COMPRESSION_LEVEL_8);
	mCompressionLevels.append(COMPRESSION_LEVEL_9);
	mCompressionLevels.append(COMPRESSION_LEVEL_10);
	mCompressionLevels.append(COMPRESSION_LEVEL_11);
	mCompressionLevels.append(COMPRESSION_LEVEL_12);
	mCompressionLevels.append(COMPRESSION_LEVEL_13);
	mCompressionLevels.append(COMPRESSION_LEVEL_14);
	for(int i = 0; i < COMPRESSIONS; ++i)
	{
		mForms.append(ViWaveForm());
		mForms[i].setCompression(mCompressionLevels[i]);
	}
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
			for(int j = 0; j < mForms.size(); ++j)
			{
				mForms[j].append(data[i]);
			}
			mFormMutex.unlock();
		}

		emit tileAvailable();
		mMutex.lock();
		isEmpty = mChunks.isEmpty();
		mMutex.unlock();
	}
}

void ViWaveWidgetThread::analyze(int size)
{
	mWidget->engine()->calculateWaveForm(mBufferType, mPosition, size);
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
	mCurrentCompressionLevel = 10;
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

	int halfHeight = height() / 2;
	int halfWidth = width() / 2;
	int position = mThread->mPosition / mThread->mCompressionLevels[mCurrentCompressionLevel];
	int start = position - halfWidth;
	int end = position + halfWidth;
	if(start < 0)
	{
		start = 0;
	}
	if(end > mThread->mForms[mCurrentCompressionLevel].size())
	{
		end = mThread->mForms[mCurrentCompressionLevel].size();
	}
	int drawStart = halfWidth + (start - position);

	for(int i = start; i < end; ++i)
	{
		painter.setPen(penNormal);
		mThread->mFormMutex.lock();
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].maximum(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].minimum(i)));
		mThread->mFormMutex.unlock();

		painter.setPen(penAverage);
		mThread->mFormMutex.lock();
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].maximumAverage(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].minimumAverage(i)));
		mThread->mFormMutex.unlock();
		drawStart++;
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
	qint8 level = mCurrentCompressionLevel + 1;
	if(level < mThread->mCompressionLevels.size())
	{
		mCurrentCompressionLevel = level;
	}
	repaint();
}

void ViWaveWidget::zoomOut()
{
	qint8 level = mCurrentCompressionLevel - 1;
	if(level >= 0)
	{
		mCurrentCompressionLevel = level;
	}
	repaint();
}
