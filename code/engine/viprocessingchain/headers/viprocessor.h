#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include "viid.h"
#include "viaudio.h"
#include "vichunk.h"
#include "viaudioformat.h"
#include "vilogger.h"
#include "vibuffer.h"
#include "viaudioobject.h"

class ViProcessor : public QObject, public QRunnable, public ViId
{
	Q_OBJECT

	signals:

		void changed();
		void progressed(short progress);
		void finished();

	public slots:

		virtual void enable();
		virtual void disable();

	public:

		ViProcessor();

		virtual void setObject(ViAudioObject *object);

		virtual void setWindowSize(int windowSize);
		virtual int windowSize();

		virtual void setFormat(ViAudioFormat format);
		virtual ViAudioFormat format();

		virtual void initialize();
		virtual void finalize();

		virtual void run() = 0;

	protected:

		ViAudioObject *mObject;

		ViAudioFormat mFormat;
		int mWindowSize;

		bool mIsEnabled;
		QMutex mIsEnabledMutex;

};

class ViObserver : public ViProcessor
{

	public:

		ViObserver();
		virtual void setData(const ViSampleChunk *data);

	protected:

		const ViSampleChunk *mData;

};

class ViDualObserver : public ViObserver
{

	public:

		ViDualObserver();
		virtual void setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData);

	protected:

		const ViSampleChunk *mData2;

};

class ViModifier : public ViProcessor
{

	public:

		ViModifier();
		virtual void setData(ViSampleChunk *data);

	protected:

		ViSampleChunk *mData;

};

#endif
