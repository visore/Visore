#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

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
		double pcmToReal8(char character);
		double pcmToReal16(char character);
		double pcmToReal32(char character);

	private:
		ViAudioMetaData *mMetaData;
		double (ViWaveFormerThread::*pcmToReal)(char); //Function pointer
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
