#ifdef VIPCMCONVERTER_H

#include "visizeconverter.h"

#define PCM_8_MAX_VALUE 127u;
#define PCM_8_MAX_AMPLITUDE 128u;
#define PCM_16_MAX_VALUE 32767u;
#define PCM_16_MAX_AMPLITUDE 32768u;
#define PCM_24_MAX_VALUE 8388607u;
#define PCM_24_MAX_AMPLITUDE 8388608u;
#define PCM_32_MAX_VALUE 2147483647u;
#define PCM_32_MAX_AMPLITUDE 2147483648u;

template <typename T>
ViPcmConverter<T>::ViPcmConverter(int size)
{
	setSize(size);
}

template <typename T>
bool ViPcmConverter<T>::setSize(int size)
{
	bool result = true;
	if(size == 32)
	{
		pcmToRealPointer = &ViPcmConverter<T>::pcmToReal32;
		realToPcmPointer = &ViPcmConverter<T>::realToPcm32;
	}
	else if(size == 24)
	{
		pcmToRealPointer = &ViPcmConverter<T>::pcmToReal24;
		realToPcmPointer = &ViPcmConverter<T>::realToPcm24;
	}
	else if(size == 16)
	{
		pcmToRealPointer = &ViPcmConverter<T>::pcmToReal16;
		realToPcmPointer = &ViPcmConverter<T>::realToPcm16;
	}
	else
	{
		pcmToRealPointer = &ViPcmConverter<T>::pcmToReal8;
		realToPcmPointer = &ViPcmConverter<T>::realToPcm8;
		result = false;
	}
	if(size == 8)
	{
		result = true;
	}
	return result;
}

template <typename T>
int ViPcmConverter<T>::pcmToReal(char* buffer, T* result, int size)
{
	return pcmToRealPointer(buffer, result, size);
}

template <typename T>
int ViPcmConverter<T>::realToPcm(T* buffer, char* result, int size)
{
	return realToPcmPointer(buffer, result, size);
}

template <typename T>
T ViPcmConverter<T>::pcmToReal8(qint8 pcm)
{
    return T(pcm) / PCM_8_MAX_VALUE;
}

template <typename T>
qint8 ViPcmConverter<T>::realToPcm8(T real)
{
    return real * PCM_8_MAX_AMPLITUDE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal16(qint16 pcm)
{
    return T(pcm) / PCM_16_MAX_VALUE;
}

template <typename T>
qint16 ViPcmConverter<T>::realToPcm16(T real)
{
    return real * PCM_16_MAX_AMPLITUDE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal24(qint32 pcm)
{
    return T(pcm) / PCM_24_MAX_VALUE;
}

template <typename T>
qint32 ViPcmConverter<T>::realToPcm24(T real)
{
    return real * PCM_24_MAX_AMPLITUDE;
}

template <typename T>
T ViPcmConverter<T>::pcmToReal32(qint32 pcm)
{
    return T(pcm) / PCM_32_MAX_VALUE;
}

template <typename T>
qint32 ViPcmConverter<T>::realToPcm32(T real)
{
    return real * PCM_32_MAX_AMPLITUDE;
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
	size = ViSizeConverter<qint16, char>::convert(size);
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
	/*size = ViPcmConverter<qint32>::sizeFromChar(size);
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal16(base[i]);
	}
	return size;*/
}

template <typename T>
int ViPcmConverter<T>::pcmToReal32(char* buffer, T *result, int size)
{
	size = ViSizeConverter<qint32, char>::convert(size);
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::pcmToReal32(base[i]);
	}
	return size;
}

template <typename T>
int ViPcmConverter<T>::realToPcm8(T* buffer, char* result, int size)
{
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::realToPcm8(buffer[i]);
	}
	return size;
}

template <typename T>
int ViPcmConverter<T>::realToPcm16(T* buffer, char* result, int size)
{
	qint16 *base = reinterpret_cast<qint16*>(result);
	for(int i = 0; i < size; ++i)
	{
		base[i] = ViPcmConverter<T>::realToPcm16(buffer[i]);
	}
	return ViSizeConverter<char, qint16>::convert(size);
}

template <typename T>
int ViPcmConverter<T>::realToPcm24(T* buffer, char* result, int size)
{
	/*qint16 *base = reinterpret_cast<qint16*>(result);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter<T>::realToPcm24(buffer[i]);
	}
	return size*3;*/
}

template <typename T>
int ViPcmConverter<T>::realToPcm32(T* buffer, char* result, int size)
{
	qint32 *base = reinterpret_cast<qint32*>(result);
	for(int i = 0; i < size; ++i)
	{
		base[i] = ViPcmConverter<T>::realToPcm32(buffer[i]);
	}
	return ViSizeConverter<char, qint32>::convert(size);
}


#endif
