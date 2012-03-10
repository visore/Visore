#ifndef VIWAVEFORMTILE_H
#define VIWAVEFORMTILE_H

#define TILE_HEIGHT 300
#define TILE_HALF_HEIGHT (TILE_HEIGHT / 2)

#include <QImage>
#include <QPainter>
#include "viwave.h"
#include "vithememanager.h"

class ViWaveFormTile
{
	public:
		ViWaveFormTile(ViWave *wave);
		~ViWaveFormTile();
		QImage* tile();

	private:
		void paintTile(ViWave *wave);

	private:
		QImage *mTile;
		qint32 mSamples;
};

#endif
