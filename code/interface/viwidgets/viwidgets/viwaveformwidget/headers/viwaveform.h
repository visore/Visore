#ifndef ViWaveForm_H
#define ViWaveForm_H

#define TILE_WIDTH 500

#include "viwaveformtile.h"
#include <qmath.h>

class ViWaveForm
{
	public:
		ViWaveForm();
		~ViWaveForm();
		void append(qreal value);
		qint32 size();
		qreal maximum(qint32 position);
		qreal minimum(qint32 position);
		qreal maximumAverage(qint32 position);
		qreal minimumAverage(qint32 position);
		void setCompression(qint32 compression);
		void reset();
		void removeFirst();
		QList<ViWaveFormTile*> mTiles;
		//Retrieves all tiles from "from", to "to" and chnages the value of offset to the value of pixels/samples of the first tile that isn't part of the range [from, to]
		QList<QImage*> tiles(qint64 from, qint64 to, qint32 *offset);

	private:
		void createTile();

	private:
		//QList<ViWaveFormTile*> mTiles;
		ViWave mWave;
		qreal mMaximum;
		qreal mMinimum;
		qreal mAverageMaximum;
		qreal mAverageMinimum;
		qint64 mMaximumCounter;
		qint64 mMinimumCounter;
		qint32 mTotalCounter;
		qint32 mCompression;
};

#endif
