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
		ViWaveFormerThread(ViAudioBuffer *buffer, QList<int> *sizes);
		void run();
};

class ViWaveFormer : public ViProcessor
{
	Q_OBJECT

	signals:
		void completed(QList<double> values);

	public:
		ViWaveFormer();
		~ViWaveFormer();
		void initialize(ViAudioBuffer *buffer);
};

#endif
