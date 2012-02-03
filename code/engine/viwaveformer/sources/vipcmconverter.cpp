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
