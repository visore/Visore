#ifndef VIWAVEFORMTILE_H
#define VIWAVEFORMTILE_H

#include <QImage>
#include <QPaintEngine>
#include <QList>

class ViWaveFormTile
{
	public:
		ViWaveFormTile(QList<double> values);
		~ViWaveFormTile();
		void draw(int height);
		QImage* tile();

	private:
		void free();
		
	private:
		QImage *mTile;
		QList<double> mValues;
};

#endif
