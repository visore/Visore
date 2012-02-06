#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#define COMPRESSION_LEVEL 200 //Combine every n milliseonds

#include "viprocessor.h"
#include "viobject.h"
#include "vipcmconverter.h"
#include <QList>

class ViWaveFormerThread : public ViProcessorThread
{
	Q_OBJECT

	signals:
		void completed(QList<double> values);

	public:
		ViWaveFormerThread(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QList<int> *sizes);
		void run();

	private:
		QList<double> pcmToReal8(char* buffer, int size);
		QList<double> pcmToReal16(char* buffer, int size);
		QList<double> pcmToReal32(char* buffer, int size);

	private:
		ViAudioMetaData *mMetaData;
		QList<double> (ViWaveFormerThread::*pcmToReal)(char*, int); //Function pointer
};

class ViWaveFormer : public ViProcessor
{
	Q_OBJECT

	signals:
		void completed(QList<double> values);

	public:
		ViWaveFormer(ViAudioMetaData *metaData);
		~ViWaveFormer();
		void initialize(ViAudioBuffer *buffer);

	private:
		ViAudioMetaData *mMetaData;
};

#endif
