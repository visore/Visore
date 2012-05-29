#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include "viid.h"
#include "vichunk.h"
#include "viaudioformat.h"

class ViProcessor : public QObject, public QRunnable, public ViId
{
	Q_OBJECT

	signals:

		void changed();
		void progressed(short progress);
		void finished();

	public:

		ViProcessor();

		virtual void setWindowSize(int windowSize);
		virtual int windowSize();

		virtual void setFormat(ViAudioFormat format);
		virtual ViAudioFormat format();

		virtual void run() = 0;

	protected:

		ViAudioFormat mFormat;
		int mWindowSize;

};

class ViObserver : public ViProcessor
{

	public:

		ViObserver();
		virtual void setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData = NULL);
		virtual void setInputData(const ViSampleChunk *data);
		virtual void setOutputData(const ViSampleChunk *data);

	protected:

		const ViSampleChunk *mInputData;
		const ViSampleChunk *mOutputData;

};

class ViModifier : public ViProcessor
{

	public:

		ViModifier();
		virtual void setData(ViSampleChunk *inputData, ViSampleChunk *outputData = NULL);
		virtual void setInputData(ViSampleChunk *data);
		virtual void setOutputData(ViSampleChunk *data);

	protected:

		ViSampleChunk *mInputData;
		ViSampleChunk *mOutputData;

};

#endif
