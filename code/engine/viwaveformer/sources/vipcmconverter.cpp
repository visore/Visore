#include "vipcmconverter.h"

#define PCMS_8_MAX_VALUE 127u; // (2^8)/2
#define PCMS_8_MAX_AMPLITUDE 128u;

#define PCMS_16_MAX_VALUE 32767u;
#define PCMS_16_MAX_AMPLITUDE 32768u;

#define PCMS_24_MAX_VALUE 8388607u;
#define PCMS_24_MAX_AMPLITUDE 32768u;

#define PCMS_32_MAX_VALUE 2147483647u;
#define PCMS_32_MAX_AMPLITUDE 2147483648u;

float ViPcmConverter::pcmToReal8(qint8 pcm)
{
    return float(pcm) / PCMS_8_MAX_AMPLITUDE;
}

qint8 ViPcmConverter::realToPcm8(float real)
{
    return real * PCMS_8_MAX_VALUE;
}

float ViPcmConverter::pcmToReal16(qint16 pcm)
{
    return float(pcm) / PCMS_16_MAX_AMPLITUDE;
}

qint16 ViPcmConverter::realToPcm16(float real)
{
    return real * PCMS_16_MAX_VALUE;
}

float ViPcmConverter::pcmToReal24(qint32 pcm)
{
    return float(pcm) / PCMS_24_MAX_AMPLITUDE;
}

qint32 ViPcmConverter::realToPcm24(float real)
{
    return real * PCMS_24_MAX_VALUE;
}

float ViPcmConverter::pcmToReal32(qint32 pcm)
{
    return float(pcm) / PCMS_32_MAX_AMPLITUDE;
}

qint32 ViPcmConverter::realToPcm32(float real)
{
    return real * PCMS_32_MAX_VALUE;
}

int ViPcmConverter::pcmToReal8(char* buffer, float *result, int size)
{
	qint8 *base = reinterpret_cast<qint8*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal8(base[i]);
	}
	return size;
}

int ViPcmConverter::pcmToReal16(char* buffer, float *result, int size)
{
	size /= 2; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal16(base[i]);
	}
	return size;
}

int ViPcmConverter::pcmToReal24(char* buffer, float *result, int size)
{
	size /= 3; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal16(base[i]);
	}
	return size;
}

int ViPcmConverter::pcmToReal32(char* buffer, float *result, int size)
{
	size /= 4; //char is only 8bit
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal32(base[i]);
	}
	return size;
}
