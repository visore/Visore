#include "vipcmconverter.h"

double ViPcmConverter::pcmToReal8(qint8 pcm)
{
    return double(pcm) / PCMS_8_MAX_AMPLITUDE;
}

qint8 ViPcmConverter::realToPcm8(double real)
{
    return real * PCMS_8_MAX_VALUE;
}

double ViPcmConverter::pcmToReal16(qint16 pcm)
{
    return double(pcm) / PCMS_16_MAX_AMPLITUDE;
}

qint16 ViPcmConverter::realToPcm16(double real)
{
    return real * PCMS_16_MAX_VALUE;
}

double ViPcmConverter::pcmToReal32(qint32 pcm)
{
    return double(pcm) / PCMS_32_MAX_AMPLITUDE;
}

qint32 ViPcmConverter::realToPcm32(double real)
{
    return real * PCMS_32_MAX_VALUE;
}

int ViPcmConverter::pcmToReal8(char* buffer, double *result, int size)
{
	qint8 *base = reinterpret_cast<qint8*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal8(base[i]);
	}
	return size;
}

int ViPcmConverter::pcmToReal16(char* buffer, double *result, int size)
{
	size /= 2; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal16(base[i]);
	}
	return size;
}

int ViPcmConverter::pcmToReal32(char* buffer, double *result, int size)
{
	size /= 4; //char is only 8bit
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal32(base[i]);
	}
	return size;
}
