#ifndef VIAUDIOMETADATA_H
#define VIAUDIOMETADATA_H

#include "viaudioformat.h"

struct ViAudioLevel
{
	public:
		ViAudioLevel(int left = 0, int right = 0)
		{
			mLeft = left;
			mRight = right;
		}
		
		void setLeft(int left)
		{
			mLeft = left;
		}

		void setRight(int right)
		{
			mRight = right;
		}

		int left()
		{
			return mLeft;
		}

		int right()
		{
			return mRight;
		}

	private:
		int mLeft;
		int mRight;
};

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

		void setLevel(ViAudioLevel level)
		{
			mLevel = level;
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

		ViAudioLevel level()
		{
			return mLevel;
		}

	private:
		ViAudioFormat mFormat;
		int mFrequency;
		int mChannels;
		qint64 mBytes;
		qint64 mMilliseconds;
		ViAudioLevel mLevel;
};

#endif
