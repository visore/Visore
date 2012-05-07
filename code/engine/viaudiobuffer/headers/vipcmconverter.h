#ifndef VIPCMCONVERTER_H
#define VIPCMCONVERTER_H

#include <QtGlobal>

template <typename T>
class ViPcmConverter
{
	public:

		static T pcmToReal8(qint8 pcm);
		static qint8 realToPcm8(T real);

		static T pcmToReal16(qint16 pcm);
		static qint16 realToPcm16(T real);

		static T pcmToReal24(qint32 pcm);
		static qint32 realToPcm24(T real);

		static T pcmToReal32(qint32 pcm);
		static qint32 realToPcm32(T real);

		static int pcmToReal8(char* buffer, T* result, int size);
		static int pcmToReal16(char* buffer, T* result, int size);
		static int pcmToReal24(char* buffer, T* result, int size);
		static int pcmToReal32(char* buffer, T* result, int size);

		static int realToPcm8(T* buffer, char* result, int size);
		static int realToPcm16(T* buffer, char* result, int size);
		static int realToPcm24(T* buffer, char* result, int size);
		static int realToPcm32(T* buffer, char* result, int size);
};

#include "../sources/vipcmconverter.cpp"

#endif
