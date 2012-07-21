#ifndef VIAUDIOPOSITION_H
#define VIAUDIOPOSITION_H

#include <QtGlobal>
#include <viaudioformat.h>

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
		ViAudioPosition(const qreal microseconds, const ViAudioPosition::Unit unit, const ViAudioFormat format);

		static qreal convertPosition(const qreal position, const ViAudioPosition::Unit fromUnit, const ViAudioPosition::Unit toUnit, const ViAudioFormat &format);

		void setPosition(const qreal position, const ViAudioPosition::Unit unit, const ViAudioFormat format);
		void setPosition(const qreal position, const ViAudioPosition::Unit unit);
		qreal position(const ViAudioPosition::Unit unit) const;

		ViAudioFormat format() const;
		
		bool operator ==(const ViAudioPosition &other) const;
		bool operator !=(const ViAudioPosition &other) const;

	private:
		qreal mMicroseconds;
		ViAudioFormat mFormat;
};

#endif
