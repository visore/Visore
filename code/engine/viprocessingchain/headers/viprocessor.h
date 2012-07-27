#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include "viid.h"
#include "viaudio.h"
#include "vichunk.h"
#include "viaudioformat.h"
#include "vilogger.h"
#include "viaudiobuffer.h"

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

		virtual void setBuffer(ViAudioBuffer *buffer, ViAudio::Mode mode = ViAudio::AudioInput) = 0;

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

		bool mIsEnabled;
		QMutex mIsEnabledMutex;

};

class ViObserver : public ViProcessor
{

	public:

		ViObserver();
		virtual void setBuffer(ViAudioBuffer *buffer, ViAudio::Mode mode);
		virtual void setData(const ViSampleChunk *data);

	protected:

		ViAudioBuffer *mBuffer;
		const ViSampleChunk *mData;

};

class ViModifier : public ViProcessor
{

	public:

		ViModifier();
		virtual void setBuffer(ViAudioBuffer *buffer, ViAudio::Mode mode);
		virtual void setData(ViSampleChunk *data);

	protected:

		ViAudioBuffer *mBuffer;
		ViSampleChunk *mData;

};

class ViDualObserver : public ViObserver
{

	public:

		ViDualObserver();
		virtual void setBuffer(ViAudioBuffer *buffer, ViAudio::Mode mode);
		virtual void setData(const ViSampleChunk *data, const ViSampleChunk *data2);

	protected:

		ViAudioBuffer *mBuffer2;
		const ViSampleChunk *mData2;

};

#endif
