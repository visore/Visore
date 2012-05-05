#ifndef VIPCMCONVERTER_H
#define VIPCMCONVERTER_H

#include <QtGlobal>

class ViPcmConverter
{
	public:
		static float pcmToReal8(qint8 pcm);
		static qint8 realToPcm8(float real);

		static float pcmToReal16(qint16 pcm);
		static qint16 realToPcm16(float real);

		static float pcmToReal24(qint32 pcm);
		static qint32 realToPcm24(float real);

		static float pcmToReal32(qint32 pcm);
		static qint32 realToPcm32(float real);

		static int pcmToReal8(char* buffer, float *result, int size);
		static int pcmToReal16(char* buffer, float *result, int size);
		static int pcmToReal24(char* buffer, float *result, int size);
		static int pcmToReal32(char* buffer, float *result, int size);
};

#endif
