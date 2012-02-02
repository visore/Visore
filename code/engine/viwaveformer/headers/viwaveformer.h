#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viprocessor.h"
#include "viobject.h"
#include <QList>

class ViWaveFormerThread : public ViProcessorThread
{
	Q_OBJECT

	signals:
		void completed(QList<int> values);

	public:
		ViWaveFormerThread(ViAudioBuffer *buffer);
		void update(int size);
};

class ViWaveFormer : public ViProcessor
{
	Q_OBJECT

	signals:
		void completed(QList<int> values);

	public:
		ViWaveFormer();
		~ViWaveFormer();
		void initialize(ViAudioBuffer *buffer);
};

#endif
