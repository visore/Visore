#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viaudiobuffer.h"

typedef ViChunk<float> ViWaveFormChunk;

class ViWaveFormer
{

	public:
		ViWaveFormer(int sampleSize = 16);
		void setSampleSize(int size);
		int pcmToReal(ViAudioBufferChunk *chunkIn, ViWaveFormChunk *chunkOut);
		int pcmToReal(ViAudioBufferChunk *chunkIn, float *dataOut);
		int pcmToReal(char *dataIn, float *dataOut, int size);

	private:
		int pcmToReal8(char* buffer, float *result, int size);
		int pcmToReal16(char* buffer, float *result, int size);
		int pcmToReal24(char* buffer, float *result, int size);
		int pcmToReal32(char* buffer, float *result, int size);

	private:
		int (ViWaveFormer::*pcmToRealPointer)(char*, float*, int); //Function pointer
		int mSampleSize;
};

#endif
