#include "viwaveformwidget.h"

ViWaveFormWidgetThread::ViWaveFormWidgetThread(ViWaveFormWidget *widget)
	: QThread()
{
	mWidget = widget;
	mPosition = -1;
	mRemains = new ViWaveFormChunk();
}

void ViWaveFormWidgetThread::run()
{
	mMutex.lock();
	while(!mChunks.isEmpty())
	{
		ViWaveFormChunk *chunk = mChunks.takeFirst();
		qreal *data = chunk->data();
		for(int i = 0; i < chunk->size(); ++i)
		{
			mForm.append(data[i]);
		}
		delete chunk;
		emit tileAvailable();
	}
	mWidget->analyze();
	mMutex.unlock();
}

void ViWaveFormWidgetThread::changeSize()
{
	//quit();
	//mForm.reset();
	mWidget->analyze();
}

void ViWaveFormWidgetThread::setCompression(qint32 compression)
{
	mForm.setCompression(compression);
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
	mWidget->analyze();
	emit tileAvailable();
}

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mParent = parent;
	mThread = new ViWaveFormWidgetThread(this);
	ViObject::connect(mEngine, SIGNAL(waveFormChanged(ViWaveFormChunk*)), mThread, SLOT(changed(ViWaveFormChunk*)));
	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), mThread, SLOT(positionChanged(ViAudioPosition)));
	ViObject::connectQueued(mThread, SIGNAL(tileAvailable()), this, SLOT(repaint()));
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
	mCompressionLevels.append(COMPRESSION_LEVEL_15);
	mCompressionLevels.append(COMPRESSION_LEVEL_16);
	mCurrentCompressionLevel = 4;
	mThread->setCompression(mCompressionLevels[mCurrentCompressionLevel]);
	mOldPosition = 0;
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	mThread->quit();
	delete mThread;
}

void ViWaveFormWidget::analyze()
{
	int realPosition = mThread->mPosition / mCompressionLevels[mCurrentCompressionLevel];
	if(mThread->mPosition >= 0)
	{
		/*int dif = realPosition - mOldPosition;
		while(dif > 0 && realPosition / mCompressionLevels[mCurrentCompressionLevel] > width() / 2)
		{
			cout<<"remove: "<<dif/ mCompressionLevels[mCurrentCompressionLevel]<<endl;
			mThread->mForm.removeFirst();
			mOldPosition = realPosition;
			//analyze();
		}*/
	//	cout<<mThread->mForm.size()<<"  "<<width()<<endl;
		if(realPosition < width() / 2 && mThread->mForm.size() <= width() / 2)
		{cout<<mThread->mForm.size()<<"  "<<width()<<endl;
			mEngine->calculateWaveForm(mThread->mPosition, mCompressionLevels[mCurrentCompressionLevel]);
			
		}
		else if(realPosition >= width() / 2)
		{
			cout<<realPosition - mOldPosition<<endl;
			int dif = realPosition - mOldPosition;
			if(dif >= 1)
			{
				mEngine->calculateWaveForm(mThread->mPosition, mCompressionLevels[mCurrentCompressionLevel]);
			}
			mOldPosition = realPosition;
		}
	}
}

void ViWaveFormWidget::resizeEvent(QResizeEvent *event)
{
	mThread->mForm.setBufferSize(width());
	mThread->changeSize();
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	mThread->mForm.setBufferSize(width());

	resize(mParent->width(), mParent->height());
	QPainter painter(this);
	painter.fillRect(rect(), ViThemeManager::color(0));

	static QPen penNormal(ViThemeManager::color(15));
	static QPen penAverage(ViThemeManager::color(14));
	static QPen penPosition(ViThemeManager::color(12));

	int halfHeight = height() / 2;
	int halfWidth = width() / 2;
	int position = mThread->mPosition / mCompressionLevels[mCurrentCompressionLevel];
	int start = position - halfWidth;
	int end = position + halfWidth;
	if(start < 0)
	{
		start = 0;
	}
	if(end > mThread->mForm.size())
	{
		end = mThread->mForm.size();
	}
	int drawStart = halfWidth + (start - position);
	for(int i = start; i < end; ++i)
	{
		painter.setPen(penNormal);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForm.maximum(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForm.minimum(i)));

		painter.setPen(penAverage);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForm.maximumAverage(i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mForm.minimumAverage(i)));
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

void ViWaveFormWidget::zoomIn()
{
	qint8 level = mCurrentCompressionLevel - 1;
	if(level >= 0)
	{
		mCurrentCompressionLevel = level;
		mThread->setCompression(mCompressionLevels[mCurrentCompressionLevel]);
	}
}

void ViWaveFormWidget::zoomOut()
{
	qint8 level = mCurrentCompressionLevel + 1;
	if(level < mCompressionLevels.size())
	{
		mCurrentCompressionLevel = level;
		mThread->setCompression(mCompressionLevels[mCurrentCompressionLevel]);
	}
}
