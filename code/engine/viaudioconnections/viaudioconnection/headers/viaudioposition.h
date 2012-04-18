#ifndef VIAUDIOPOSITION_H
#define VIAUDIOPOSITION_H

#include <QtGlobal>
#include <QAudioFormat>

class ViAudioPosition
{
	public:
		ViAudioPosition();
		ViAudioPosition(qint64 microseconds, QAudioFormat format);

		void initialize(qint64 microseconds, QAudioFormat format);

		qreal seconds();
		qreal milliseconds();
		qint64 microseconds();
		qint64 bytes();
		qint64 sample();

		QAudioFormat format();
		
		bool operator ==(const ViAudioPosition &other) const;
		bool operator !=(const ViAudioPosition &other) const;

	private:
		qint64 mMicroseconds;
		QAudioFormat mFormat;
};

#endif
