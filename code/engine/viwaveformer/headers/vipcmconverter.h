#ifndef VIPCMCONVERTER_H
#define VIPCMCONVERTER_H

#define PCMS_8_MAX_VALUE 128; // (2^8)/2
#define PCMS_8_MAX_AMPLITUDE 128;

#define PCMS_16_MAX_VALUE 32767;
#define PCMS_16_MAX_AMPLITUDE 32768; // because minimum is -32768

#define PCMS_32_MAX_VALUE 2147483648;
#define PCMS_32_MAX_AMPLITUDE 2147483648;

#include <QtGlobal>

class ViPcmConverter
{
	public:
		static double pcmToReal8(qint8 pcm);
		static qint8 realToPcm8(double real);
		static double pcmToReal16(qint16 pcm);
		static qint16 realToPcm16(double real);
		static double pcmToReal32(qint32 pcm);
		static qint32 realToPcm32(double real);
};

#endif
