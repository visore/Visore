#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#define COMPRESSION_LEVEL 200 //Combine every n milliseonds

#include "viprocessor.h"
#include "viobject.h"
#include "vipcmconverter.h"
#include <QList>
#include <QMetaType>

#include <iostream>
using namespace std;

struct ViWaveFormChunk
{
	ViWaveFormChunk()
	{
		mData = NULL;
		mSize = 0;
	}
	ViWaveFormChunk(double *data, int size)
	{
		mData = data;
		mSize = size;
	}
	~ViWaveFormChunk()
	{
		clear();
	}
	void clear()
	{
		if(mData != NULL)
		{
			delete [] mData;
			mData = NULL;
		}
		mSize = 0;
	}
	double *mData;
	int mSize;
};

class ViWaveFormerThread : public ViProcessorThread
{
	Q_OBJECT

	signals:
		void completed(ViWaveFormChunk *chunk);

	public:
		ViWaveFormerThread(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QList<int> *sizes);
		void run();

	private:
		int pcmToReal8(char* buffer, double *result, int size);
		int pcmToReal16(char* buffer, double *result, int size);
		int pcmToReal32(char* buffer, double *result, int size);

	private:
		ViAudioMetaData *mMetaData;
		int (ViWaveFormerThread::*pcmToReal)(char*, double*, int); //Function pointer
};

class ViWaveFormer : public ViProcessor
{
	Q_OBJECT

	signals:
		void completed(ViWaveFormChunk *chunk);

	public:
		ViWaveFormer(ViAudioMetaData *metaData);
		~ViWaveFormer();
		void initialize(ViAudioBuffer *buffer);

	private:
		ViAudioMetaData *mMetaData;
};

#endif
