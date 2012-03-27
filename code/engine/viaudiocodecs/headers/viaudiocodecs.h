#ifndef VIAUDIOCODECS_H
#define VIAUDIOCODECS_H

#define __STDC_CONSTANT_MACROS

#include <QFile>
#include "viaudiobufferstream.h"

class ViAudioCodecs : public QObject
{
    Q_OBJECT

	public:
		enum Result
		{
			Success = 0,
			CodecNotFound = 1,
			CodecNotOpened = 2,
			FileNotOpened = 3
		};

	public:
		static ViAudioCodecs::Result encode(ViAudioBuffer *bufferIn, ViAudioBuffer *bufferOut);
		static ViAudioCodecs::Result encode(ViAudioBuffer *bufferIn, QString outputFilePath);

	private:
		static void initlialize();
		
};

#endif
