#include "viwaveformtile.h"

ViWaveFormTile::ViWaveFormTile(ViWave *wave)
{
	mSamples = wave->size();
	QImage::Format format = QImage::Format_ARGB4444_Premultiplied;
    mTile = new QImage(mSamples, TILE_HEIGHT, format);
	mTile->fill(qRgba(255, 255, 255, 0));
	paintTile(wave);
}

ViWaveFormTile::~ViWaveFormTile()
{
	delete mTile;
}

QImage* ViWaveFormTile::tile()
{
	return mTile;
}

void ViWaveFormTile::paintTile(ViWave *wave)
{
	static QPen penNormal(ViThemeManager::color(15));
	static QPen penAverage(ViThemeManager::color(14));

	QPainter painter(mTile);
	for(int i = 0; i < mSamples; ++i)
	{
		/*painter.setPen(penNormal);
		painter.drawLine(i, TILE_HALF_HEIGHT, i, TILE_HALF_HEIGHT - (TILE_HALF_HEIGHT * wave->at(i)->maximum()));
		painter.drawLine(i, TILE_HALF_HEIGHT, i, TILE_HALF_HEIGHT - (TILE_HALF_HEIGHT * wave->at(i)->minimum()));

		painter.setPen(penAverage);
		painter.drawLine(i, TILE_HALF_HEIGHT, i, TILE_HALF_HEIGHT - (TILE_HALF_HEIGHT * wave->at(i)->maximumAverage()));
		painter.drawLine(i, TILE_HALF_HEIGHT, i, TILE_HALF_HEIGHT - (TILE_HALF_HEIGHT * wave->at(i)->minimumAverage()));*/
	}
	painter.end();
}
