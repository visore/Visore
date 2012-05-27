#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <QAudioFormat>
#include "vicodec.h"

class ViAudioFormat : public QAudioFormat
{
	public:

		ViAudioFormat();
		ViAudioFormat(const ViAudioFormat &format);

		static ViAudioFormat defaultFormat();

		void setCodec(ViCodec codec);
		ViCodec codec();

		void setBitRate(int bitRate);
		int bitRate();

	private:

		ViCodec mCodec;
		int mBitRate;
};

#endif
