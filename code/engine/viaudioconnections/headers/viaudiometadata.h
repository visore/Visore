#ifndef VIAUDIOMETADATA_H
#define VIAUDIOMETADATA_H

#include "viaudioformat.h"

class ViAudioMetaData
{
	public:
		void setFormat(ViAudioFormat format)
		{
			mFormat = format;
		}

		void setFrequency(int frequency)
		{
			mFrequency = frequency;
		}

		void setChannels(int channels)
		{
			mChannels = channels;
		}

		void setBytes(qint64 bytes)
		{
			mBytes = bytes;
		}

		void setMilliseconds(qint64 milliseconds)
		{
			mMilliseconds = milliseconds;
		}

		void setSeconds(qint64 seconds)
		{
			mMilliseconds = seconds * 1000;
		}

		ViAudioFormat format()
		{
			return mFormat;
		}

		int frequency()
		{
			return mFrequency;
		}

		int channels()
		{
			return mChannels;
		}

		qint64 bytes()
		{
			return mBytes;
		}

		qint64 milliseconds()
		{
			return mMilliseconds;
		}

		qint64 seconds()
		{
			return mMilliseconds / 1000;
		}

	private:
		ViAudioFormat mFormat;
		int mFrequency;
		int mChannels;
		qint64 mBytes;
		qint64 mMilliseconds;
};

#endif
