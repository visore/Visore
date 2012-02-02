#ifndef VIPCMCONVERTER_H
#define VIPCMCONVERTER_H

#define PCMS_16_MAX_VALUE 32767;
#define PCMS_16_MAX_AMPLITUDE 32768; // because minimum is -32768

#include <QtGlobal>

class ViPcmConverter
{
	public:
		static double pcmToReal(qint16 pcm);
		static qint16 realToPcm(double real);
};

#endif
