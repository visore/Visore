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

		double array[chunk->mSize + mRemains->mSize];
		for(int i = 0; i < mRemains->mSize; ++i)
		{
			array[i] = mRemains->mData[i];
		}
		for(int i = 0; i < chunk->mSize; ++i)
		{
			array[mRemains->mSize + i] = chunk->mData[i];
		}

		int size = mRemains->mSize + chunk->mSize;

		delete chunk;

		for(int i = 0; i < size; i += COMPRESSION_LEVEL_3)
		{
			int remains = size - i;
			if(remains < COMPRESSION_LEVEL_3)
			{
				double *remainsData = new double[remains];
				//Not enough samples avialable to create another summarized amplitude
				for(int j = 0; j < remains; ++j)
				{
					remainsData[j] = array[i + j]; 
				}
				delete mRemains;
				mRemains = new ViWaveFormChunk(remainsData, remains);
				break;
			}
			double averageMaximum = 0;
			double averageMinimum = 0;
			double maximum = -1;
			double minimum = 1;
			int counterMaximum = 0;
			int counterMinimum = 0;
			for(int j = 0; j < COMPRESSION_LEVEL_3; ++j)
			{
				double number = array[i + j];
				if(number < 0)
				{
					averageMinimum += number;
					++counterMinimum;
				}
				else if(number > 0)
				{
					averageMaximum += number;
					++counterMaximum;
				}
				if(number > maximum)
				{
					maximum = number;
				}
				else if(number < minimum)
				{
					minimum = number;
				}
			}
			if(counterMaximum) //Make sure not to devide by 0
			{
				averageMaximum /= counterMaximum;
			}
			if(counterMinimum) //Make sure not to devide by 0
			{
				averageMinimum /= counterMinimum;
			}
			mAmplitudes.append(ViWaveAmplitude(maximum, minimum, averageMaximum, averageMinimum));
		}
	}
	mMutex.unlock();
	emit tileAvailable();
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

void ViWaveFormWidgetThread::positionChanged(qint64 bytes, qint64 milliseconds, qint8 bits)
{
	if(bits == 16)
	{
		bits = 2;
	}
	else if(bits == 32)
	{
		bits = 4;
	}
	else
	{
		bits = 1;
	}
	mPosition = bytes / (COMPRESSION_LEVEL_3 * bits);
	emit tileAvailable();
}

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	resize(parent->width(), parent->height());
	mThread = new ViWaveFormWidgetThread(this);
	ViObject::connect(mEngine, SIGNAL(waveFormChanged(ViWaveFormChunk*)), mThread, SLOT(changed(ViWaveFormChunk*)));
	ViObject::connect(mEngine, SIGNAL(positionChanged(qint64, qint64, qint8)), mThread, SLOT(positionChanged(qint64, qint64, qint8)));
	ViObject::connectQueued(mThread, SIGNAL(tileAvailable()), this, SLOT(repaint()));
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mThread;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect(), ViThemeManager::color(0));

	static QPen penNormal(ViThemeManager::color(15));
	static QPen penAverage(ViThemeManager::color(14));
	static QPen penPosition(ViThemeManager::color(12));

	int halfHeight = height() / 2;
	int halfWidth = width() / 2;
	int position = mThread->mPosition;
	int start = position - halfWidth;
	int end = position + halfWidth;
	if(start < 0)
	{
		start = 0;
	}
	if(end > mThread->mAmplitudes.size())
	{
		end = mThread->mAmplitudes.size();
	}
	int drawStart = halfWidth + (start - position);

	for(int i = start; i < end; ++i)
	{
		ViWaveAmplitude amplitude = mThread->mAmplitudes[i];

		painter.setPen(penNormal);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * amplitude.mMaximum));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * amplitude.mMinimum));

		painter.setPen(penAverage);
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * amplitude.mAverageMaximum));
		painter.drawLine(drawStart, halfHeight, drawStart, halfHeight - (halfHeight * amplitude.mAverageMinimum));
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
