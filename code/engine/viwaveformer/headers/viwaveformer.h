#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viaudiobuffer.h"

typedef ViChunk<qreal> ViWaveFormChunk;

class ViWaveFormer
{

	public:
		ViWaveFormer(int sampleSize = 16);
		void setSampleSize(int size);
		int pcmToReal(ViAudioBufferChunk *chunkIn, ViWaveFormChunk *chunkOut);
		int pcmToReal(ViAudioBufferChunk *chunkIn, qreal *dataOut);
		int pcmToReal(char *dataIn, qreal *dataOut, int size);

	private:
		int pcmToReal8(char* buffer, qreal *result, int size);
		int pcmToReal16(char* buffer, qreal *result, int size);
		int pcmToReal24(char* buffer, qreal *result, int size);
		int pcmToReal32(char* buffer, qreal *result, int size);

	private:
		int (ViWaveFormer::*pcmToRealPointer)(char*, qreal*, int); //Function pointer
		int mSampleSize;
};

#endif
