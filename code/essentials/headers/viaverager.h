#ifndef VIAVERAGER_H
#define VIAVERAGER_H

#include <QtGlobal>
#include <QList>

class ViAverager
{
	public:
		
		ViAverager();
		void add(qreal value);
		qreal average();
		void clear();

		static qreal average(QList<qreal> values);

	private:

		qint64 mCounter;
		qreal mAverage;

};

#endif
