#include "viwaveform.h"

ViWaveForm::ViWaveForm()
{
	mCompression = 0;
	reset();
}

ViWaveForm::~ViWaveForm()
{
	//qDeleteAll(mTiles);
	mTiles.clear();
}

void ViWaveForm::append(qreal value)
{
	++mTotalCounter;
	if(value > mMaximum)
	{
		mMaximum = value;
	}
	else if(value < mMinimum)
	{
		mMinimum = value;
	}
	if(value > 0)
	{
		++mMaximumCounter;
		mAverageMaximum += value;
	}
	else if(value < 0)
	{
		++mMinimumCounter;
		mAverageMinimum += value;
	}
	if(mTotalCounter == mCompression)
	{
		if(mMaximumCounter > 0)
		{
			mAverageMaximum /= mMaximumCounter;
		}
		if(mMinimumCounter > 0)
		{
			mAverageMinimum /= mMinimumCounter;
		}
		mWave.append(ViAmplitude(mMaximum, mMinimum, mAverageMaximum, mAverageMinimum));
		//createTile();
		reset();
	}
}

void ViWaveForm::createTile()
{
	if(mWave.size() == TILE_WIDTH)
	{
		mTiles.append(new ViWaveFormTile(&mWave));
		//qDeleteAll(mWave);
		mWave.clear();
	}
}

QList<QImage*> ViWaveForm::tiles(qint64 from, qint64 to, qint32 *offset)
{
	QList<QImage*> result;
	qint64 start = qFloor(qreal(from) / TILE_WIDTH);
	qint64 end = qCeil(qreal(to) / TILE_WIDTH);
	*offset = int((qreal(from) / TILE_WIDTH) - start) % TILE_WIDTH;

	if(start > mTiles.size())
	{
		start = mTiles.size();
	}
	if(end > mTiles.size())
	{
		end = mTiles.size();
	}

	for(int i = start; i < end; ++i)
	{
		result.append(mTiles[i]->tile());
	}
	return result;
}

qint32 ViWaveForm::size()
{
	return mWave.size();
}

qreal ViWaveForm::maximum(qint32 position)
{
	return mWave[position].maximum();
}

qreal ViWaveForm::minimum(qint32 position)
{
	return mWave[position].minimum();
}

qreal ViWaveForm::maximumAverage(qint32 position)
{
	return mWave[position].maximumAverage();
}

qreal ViWaveForm::minimumAverage(qint32 position)
{
	return mWave[position].minimumAverage();
}

void ViWaveForm::setCompression(qint32 compression)
{
	mCompression = compression;
}

void ViWaveForm::reset()
{
	mMaximum = -1;
	mMinimum = 1;
	mAverageMaximum = 0;
	mAverageMinimum = 0;
	mMaximumCounter = 0;
	mMinimumCounter = 0;
	mTotalCounter = 0;
}

void ViWaveForm::removeFirst()
{
	mWave.removeFirst();
}

