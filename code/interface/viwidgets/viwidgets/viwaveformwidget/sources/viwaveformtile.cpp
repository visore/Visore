#include "viwaveformtile.h"

#include <iostream>
using namespace std;

ViWaveFormTile::ViWaveFormTile()
{
	mTile = NULL;
	mPainter = NULL;
}

ViWaveFormTile::~ViWaveFormTile()
{
	if(mTile != NULL)
	{
		delete mTile;
		mTile = NULL;
	}
	if(mPainter != NULL)
	{
		delete mPainter;
		mPainter = NULL;
	}
}

void ViWaveFormTile::draw(QList<double> values, int height)
{
	mTile = new QImage(values.size(), height, QImage::Format_RGB32);
	mPainter = new QPainter(mTile);
	mPainter->fillRect(mTile->rect(), Qt::black);
	QPen pen(Qt::white);
    mPainter->setPen(pen);
	int halfHeight = height / 2;
	mPreviousX = 0;
	mPreviousY = 0;
	for(int i = 0; i < values.size(); ++i)
	{
		int x = i;
		int y = halfHeight - (halfHeight * values[i]);
		mPainter->drawLine(mPreviousX, mPreviousY, x, y);
		mPreviousX = x;
		mPreviousY = y;
	}
}

QImage* ViWaveFormTile::tile()
{
	return mTile;
}
