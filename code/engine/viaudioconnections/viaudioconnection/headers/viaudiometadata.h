#ifndef VIAUDIOMETADATA_H
#define VIAUDIOMETADATA_H

#include "viaudioformat.h"

class ViAudioMetaData
{
	public:
		void setFormat(ViAudioFormat format);
		void setFrequency(int frequency);
		void setChannels(int channels);
		void setBytes(qint64 bytes);
		void setMilliseconds(qint64 milliseconds);
		void setSeconds(qint64 seconds);
		ViAudioFormat format();
		int frequency();
		int channels();
		qint64 bytes();
		qint64 milliseconds();
		qint64 seconds();

	private:
		ViAudioFormat mFormat;
		int mFrequency;
		int mChannels;
		qint64 mBytes;
		qint64 mMilliseconds;
};

#endif
