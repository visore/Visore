#ifndef VIAUDIOPOSITION_H
#define VIAUDIOPOSITION_H

#include <QtGlobal>
#include <QAudioFormat>

class ViAudioPosition
{

	public:

		enum Unit
		{
			Microseconds = 0,
			Milliseconds = 1,
			Seconds = 2,
			Samples = 3,
			Bytes = 4
		};

	public:

		ViAudioPosition();
		ViAudioPosition(qreal microseconds, ViAudioPosition::Unit unit, QAudioFormat format);

		static qreal convertPosition(qreal position, ViAudioPosition::Unit fromUnit, ViAudioPosition::Unit toUnit, QAudioFormat format);

		void setPosition(qreal position, ViAudioPosition::Unit unit, QAudioFormat format);
		void setPosition(qreal position, ViAudioPosition::Unit unit);
		qreal position(ViAudioPosition::Unit unit);

		QAudioFormat format();
		
		bool operator ==(const ViAudioPosition &other) const;
		bool operator !=(const ViAudioPosition &other) const;

	private:
		qreal mMicroseconds;
		QAudioFormat mFormat;
};

#endif
