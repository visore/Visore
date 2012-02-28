#include "viwaveformwidget.h"

ViWaveFormWidgetThread::ViWaveFormWidgetThread(ViWaveFormWidget *widget)
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

void ViWaveFormWidgetThread::run()
{
	mMutex.lock();
	while(!mChunks.isEmpty())
	{
		ViWaveFormChunk *chunk = mChunks.takeFirst();
		mMutex.unlock();
		qreal *data = chunk->data();
		for(int i = 0; i < chunk->size(); ++i)
		{
			for(int j = 0; j < mForms.size(); ++j)
			{
				mForms[j].append(data[i]);
			}
		}
		delete chunk;
		emit tileAvailable();
	}
	mMutex.unlock();
}

void ViWaveFormWidgetThread::analyze(int size)
{
	mWidget->mEngine->calculateWaveForm(mBufferType, mPosition, size);
}

void ViWaveFormWidgetThread::changed(ViWaveFormChunk *chunk)
{
	mMutex.lock();
	mChunks.append(chunk);
	mMutex.unlock();
	if(!isRunning())
	{
		start();
	}
}

void ViWaveFormWidgetThread::positionChanged(ViAudioPosition position)
{
	mPosition = position.sample();
	emit tileAvailable();
}

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(engine, parent)
{
	mToolbar = new ViWidgetToolbar(ViWidgetToolbar::Right, engine, parent);
	mToolbar->addButton("Zoom In", ViThemeManager::icon("zoomin.png"), this, SLOT(zoomIn()));
	mToolbar->addButton("Zoom Out", ViThemeManager::icon("zoomout.png"), this, SLOT(zoomOut()));
	mThread = new ViWaveFormWidgetThread(this);
	mThread->mBufferType = type;

	if(type == ViAudioBuffer::Original)
	{
		ViObject::connect(mEngine, SIGNAL(originalWaveChanged(ViWaveFormChunk*)), mThread, SLOT(changed(ViWaveFormChunk*)));
		ViObject::connect(mEngine, SIGNAL(originalBufferChanged(int)), mThread, SLOT(analyze(int)));
	}
	else
	{
		ViObject::connect(mEngine, SIGNAL(correctedWaveChanged(ViWaveFormChunk*)), mThread, SLOT(changed(ViWaveFormChunk*)));
		ViObject::connect(mEngine, SIGNAL(correctedBufferChanged(int)), mThread, SLOT(analyze(int)));
	}

	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), mThread, SLOT(positionChanged(ViAudioPosition)));
	ViObject::connectQueued(mThread, SIGNAL(tileAvailable()), this, SLOT(repaint()));
	mCurrentCompressionLevel = 10;
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	mThread->quit();
	delete mThread;
	delete mToolbar;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	resize(mParent->width(), mParent->height());
	QPainter painter(this);
	painter.fillRect(rect(), ViThemeManager::color(0));

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
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].maximum(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].minimum(i)));

		painter.setPen(penAverage);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].maximumAverage(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForms[mCurrentCompressionLevel].minimumAverage(i)));
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

void ViWaveFormWidget::resizeEvent(QResizeEvent *event)
{
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

void ViWaveFormWidget::zoomIn()
{
	qint8 level = mCurrentCompressionLevel + 1;
	if(level < mThread->mCompressionLevels.size())
	{
		mCurrentCompressionLevel = level;
	}
	repaint();
}

void ViWaveFormWidget::zoomOut()
{
	qint8 level = mCurrentCompressionLevel - 1;
	if(level >= 0)
	{
		mCurrentCompressionLevel = level;
	}
	repaint();
}
