#ifndef VIRANGE_H
#define VIRANGE_H

#include <QtGlobal>

class ViRange
{

	public:
		
		ViRange(qreal start = 0, qreal end = 0);
		void setStart(qreal start);
		void setEnd(qreal end);
		qreal start();
		qreal end();

		// If the current range falls between the passed range.
		bool isBetween(qreal start, qreal end);
		bool isBetween(ViRange range);

		// If the given value is in the current range
		bool contains(qreal value);

	private:

		qreal mStart;
		qreal mEnd;

};

#endif
