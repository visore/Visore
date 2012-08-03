#include "virange.h"

ViRange::ViRange(qreal start, qreal end)
{
	mStart = start;
	mEnd = end;
}

void ViRange::setStart(qreal start)
{
	mStart = start;
}

void ViRange::setEnd(qreal end)
{
	mEnd = end;
}

qreal ViRange::start()
{
	return mStart;
}

qreal ViRange::end()
{
	return mEnd;
}

bool ViRange::isBetween(qreal start, qreal end)
{
	return mStart >= start && mEnd <= end;
}

bool ViRange::isBetween(ViRange range)
{
	return mStart >= range.start() && mEnd <= range.end();
}

bool ViRange::contains(qreal value)
{
	return value >= mStart && value <= mEnd;
}
