#ifndef VIAUDIOPOSITION_H
#define VIAUDIOPOSITION_H

#include <QtGlobal>

class ViAudioPosition
{
	public:
		ViAudioPosition(qint64 bytes = 0, qint8 bitDepth = 0, qreal secondsInByte = 0);
		qint64 seconds();
		qint64 milliseconds();
		qint64 bytes();
		qint8 bitDepth();
		qint64 sample();
		bool operator ==(const ViAudioPosition &other) const;
		bool operator !=(const ViAudioPosition &other) const;

	private:
		qint64 mSeconds;
		qint64 mMilliseconds;
		qint64 mBytes;
		qint8 mBitDepth;
		qint64 mSample;
};

#endif
