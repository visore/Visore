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
		void enabled(bool enabled);
		void progressed(short progress);
		void finished();

	public:

		ViProcessor();

		virtual void enable(bool enable = true);
		virtual bool isEnabled();

		virtual void setWindowSize(int windowSize);
		virtual int windowSize();

		virtual void setFormat(ViAudioFormat format);
		virtual ViAudioFormat format();

		virtual void run() = 0;

	protected:

		ViAudioFormat mFormat;
		bool mIsEnabled;
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

#endif
