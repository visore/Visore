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
			mCollection.append(data[i]);
		}
		delete chunk;
		emit tileAvailable();
	}
	mMutex.unlock();
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

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mParent = parent;
	mThread = new ViWaveFormWidgetThread(this);
	ViObject::connect(mEngine, SIGNAL(waveFormChanged(ViWaveFormChunk*)), mThread, SLOT(changed(ViWaveFormChunk*)));
	ViObject::connect(mEngine, SIGNAL(positionChanged(ViAudioPosition)), mThread, SLOT(positionChanged(ViAudioPosition)));
	ViObject::connectQueued(mThread, SIGNAL(tileAvailable()), this, SLOT(repaint()));
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	mThread->quit();
	delete mThread;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	int compression = 0;
	resize(mParent->width(), mParent->height());
	QPainter painter(this);
	painter.fillRect(rect(), ViThemeManager::color(0));

	static QPen penNormal(ViThemeManager::color(15));
	static QPen penAverage(ViThemeManager::color(14));
	static QPen penPosition(ViThemeManager::color(12));

	int halfHeight = height() / 2;
	int halfWidth = width() / 2;
	int position = mThread->mPosition / mThread->mCollection.samples(compression);
	int start = position - halfWidth;
	int end = position + halfWidth;
	if(start < 0)
	{
		start = 0;
	}
	if(end > mThread->mCollection.size(compression))
	{
		end = mThread->mCollection.size(compression);
	}
	int drawStart = halfWidth + (start - position);

	for(int i = start; i < end; ++i)
	{
		painter.setPen(penNormal);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mCollection.maximum(compression, i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mCollection.minimum(compression, i)));

		painter.setPen(penAverage);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mCollection.maximumAverage(compression, i)));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * mThread->mCollection.minimumAverage(compression, i)));
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
