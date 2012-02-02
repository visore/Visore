#include "vipcmconverter.h"

double ViPcmConverter::pcmToReal(qint16 pcm)
{
    return double(pcm) / PCMS_16_MAX_AMPLITUDE;
}

qint16 ViPcmConverter::realToPcm(double real)
{
    return real * PCMS_16_MAX_VALUE;
}
