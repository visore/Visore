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

		virtual void initialize();
		virtual void finalize();

		virtual void run() = 0;

	protected:

		ViAudioFormat mFormat;
		int mWindowSize;

};

class ViObserver : public ViProcessor
{

	public:

		ViObserver();
		virtual void setData(const ViSampleChunk *data);

	protected:

		const ViSampleChunk *mData;

};

class ViModifier : public ViProcessor
{

	public:

		ViModifier();
		virtual void setData(ViSampleChunk *data);

	protected:

		ViSampleChunk *mData;

};

class ViDualObserver : public ViObserver
{

	public:

		ViDualObserver();
		virtual void setData(const ViSampleChunk *data, const ViSampleChunk *data2);

	protected:

		const ViSampleChunk *mData2;

};

#endif
