#ifndef VIWAVEFORMTILE_H
#define VIWAVEFORMTILE_H

#include <QImage>
#include <QPaintEngine>
#include <QList>

class ViWaveFormTile
{
	public:
		ViWaveFormTile();
		~ViWaveFormTile();
		void draw(QList<double> values, int height);
		QImage* tile();
		
	private:
		QImage *mTile;
		QPainter *mPainter;
		int mPreviousX;
		int mPreviousY;
};

#endif
