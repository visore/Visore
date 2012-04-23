#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <QAudioFormat>
#include "vicodec.h"

class ViAudioFormat : public QAudioFormat
{
	public:

		void setCodec(ViCodec codec);
		ViCodec codec();

	private:

		ViCodec mCodec;
};

#endif
