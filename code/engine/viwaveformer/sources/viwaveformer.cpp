#include "viwaveformer.h"
#include "vipcmconverter.h"

#include <iostream>
using namespace std;

ViWaveFormer::ViWaveFormer(int sampleSize)
{
	setSampleSize(sampleSize);
}

void ViWaveFormer::setSampleSize(int sampleSize)
{
	mSampleSize = sampleSize;
	if(mSampleSize == 8)
	{
		pcmToRealPointer = &ViWaveFormer::pcmToReal8;
	}
	else if(mSampleSize == 16)
	{
		pcmToRealPointer = &ViWaveFormer::pcmToReal16;
	}
	else if(mSampleSize == 24)
	{
		pcmToRealPointer = &ViWaveFormer::pcmToReal24;
	}
	else if(mSampleSize == 32)
	{
		pcmToRealPointer = &ViWaveFormer::pcmToReal32;
	}
}

int ViWaveFormer::pcmToReal(ViAudioBufferChunk *chunkIn, ViWaveFormChunk *chunkOut)
{
	chunkOut->resize(chunkIn->size() / (mSampleSize / 8));
	return pcmToReal(chunkIn->data(), chunkOut->data(), chunkIn->size());
}

int ViWaveFormer::pcmToReal(ViAudioBufferChunk *chunkIn, qreal *dataOut)
{
	return pcmToReal(chunkIn->data(), dataOut, chunkIn->size());
}

int ViWaveFormer::pcmToReal(char *dataIn, qreal *dataOut, int size)
{
	return (this->*pcmToRealPointer)(dataIn, dataOut, size);
}

int ViWaveFormer::pcmToReal8(char* buffer, qreal *result, int size)
{
	return ViPcmConverter::pcmToReal8(buffer, result, size);
}

int ViWaveFormer::pcmToReal16(char* buffer, qreal *result, int size)
{
	return ViPcmConverter::pcmToReal16(buffer, result, size);
}

int ViWaveFormer::pcmToReal24(char* buffer, qreal *result, int size)
{
	return ViPcmConverter::pcmToReal24(buffer, result, size);
}

int ViWaveFormer::pcmToReal32(char* buffer, qreal *result, int size)
{
	return ViPcmConverter::pcmToReal32(buffer, result, size);
}
