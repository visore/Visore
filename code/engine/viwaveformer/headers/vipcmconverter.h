#ifndef VIPCMCONVERTER_H
#define VIPCMCONVERTER_H

#include <QtGlobal>

class ViPcmConverter
{
	public:
		static qreal pcmToReal8(qint8 pcm);
		static qint8 realToPcm8(qreal real);

		static qreal pcmToReal16(qint16 pcm);
		static qint16 realToPcm16(qreal real);

		static qreal pcmToReal24(qint32 pcm);
		static qint32 realToPcm24(qreal real);

		static qreal pcmToReal32(qint32 pcm);
		static qint32 realToPcm32(qreal real);

		static int pcmToReal8(char* buffer, qreal *result, int size);
		static int pcmToReal16(char* buffer, qreal *result, int size);
		static int pcmToReal24(char* buffer, qreal *result, int size);
		static int pcmToReal32(char* buffer, qreal *result, int size);
};

#endif
