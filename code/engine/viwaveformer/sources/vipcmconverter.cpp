#ifdef VIPCMCONVERTER_H

#define PCMS_8_MAX_VALUE 127u; // (2^8)/2
#define PCMS_8_MAX_AMPLITUDE 128u;

#define PCMS_16_MAX_VALUE 32767u;
#define PCMS_16_MAX_AMPLITUDE 32768u;

#define PCMS_24_MAX_VALUE 8388607u;
#define PCMS_24_MAX_AMPLITUDE 32768u;

#define PCMS_32_MAX_VALUE 2147483647u;
#define PCMS_32_MAX_AMPLITUDE 2147483648u;

template <typename T>
T ViPcmConverter<T>::pcmToReal8(qint8 pcm)
{
    return T(pcm) / PCMS_8_MAX_AMPLITUDE;
}

template <typename T>
qint8 ViPcmConverter<T>::realToPcm8(T real)
{
    return real * PCMS_8_MAX_VALUE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal16(qint16 pcm)
{
    return T(pcm) / PCMS_16_MAX_AMPLITUDE;
}

template <typename T>
qint16 ViPcmConverter<T>::realToPcm16(T real)
{
    return real * PCMS_16_MAX_VALUE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal24(qint32 pcm)
{
    return T(pcm) / PCMS_24_MAX_AMPLITUDE;
}

template <typename T>
qint32 ViPcmConverter<T>::realToPcm24(T real)
{
    return real * PCMS_24_MAX_VALUE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal32(qint32 pcm)
{
    return T(pcm) / PCMS_32_MAX_AMPLITUDE;
}

template <typename T>
qint32 ViPcmConverter<T>::realToPcm32(T real)
{
    return real * PCMS_32_MAX_VALUE;
}

template <typename T>
int ViPcmConverter<T>::pcmToReal8(char* buffer, T *result, int size)
{
	qint8 *base = reinterpret_cast<qint8*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal8(base[i]);
	}
	return size;
}

template <typename T>
int ViPcmConverter<T>::pcmToReal16(char* buffer, T *result, int size)
{
	size /= 2; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal16(base[i]);
	}
	return size;
}

template <typename T>
int ViPcmConverter<T>::pcmToReal24(char* buffer, T *result, int size)
{
	size /= 3; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal16(base[i]);
	}
	return size;
}

template <typename T>
int ViPcmConverter<T>::pcmToReal32(char* buffer, T *result, int size)
{
	size /= 4; //char is only 8bit
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal32(base[i]);
	}
	return size;
}

#endif
