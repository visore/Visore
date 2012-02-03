#include "viwaveformtile.h"

#include <iostream>
using namespace std;

ViWaveFormTile::ViWaveFormTile(QList<double> values)
{
	mValues = values;
	mTile = NULL;
}

ViWaveFormTile::~ViWaveFormTile()
{
	free();
}

void ViWaveFormTile::free()
{
	if(mTile != NULL)
	{
		delete mTile;
		mTile = NULL;
	}
}

void ViWaveFormTile::draw(int height)
{
	free();
	mTile = new QImage(mValues.size(), height, QImage::Format_RGB32);
	QPainter painter(mTile);
	painter.fillRect(mTile->rect(), Qt::black);
	QPen pen(Qt::white);
    painter.setPen(pen);
	int halfHeight = height / 2;
	for(int i = 0; i < mValues.size(); ++i)
	{
		painter.drawLine(i, halfHeight, i, halfHeight - (halfHeight * mValues[i]));
	}
}

QImage* ViWaveFormTile::tile()
{
	return mTile;
}
