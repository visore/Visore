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
#include "vielement.h"

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

		ViProcessor(QString name);

		virtual void setObject(ViAudioObjectPointer object);

		virtual void setWindowSize(int windowSize);
		virtual int windowSize();

		virtual void setFormat(ViAudioFormat format);
		virtual ViAudioFormat format();

		virtual void initialize();
		virtual void finalize();

		virtual void execute() = 0;
		virtual void run();

		virtual bool importParameters(const ViElement &element);
		virtual void exportParameters(ViElement &element);

		virtual bool importResults(const ViElement &element);
		virtual void exportResults(ViElement &element);

		bool isEnabled();
		bool isDisabled();

		QString name(bool simple = false);

	protected:

		ViAudioObjectPointer mObject;

		ViAudioFormat mFormat;
		int mWindowSize;

		bool mIsEnabled;
		QMutex mIsEnabledMutex;

	private:

		QString mName;

};

class ViObserver : public ViProcessor
{

	public:

		ViObserver(QString name = "");
		virtual void setData(const ViSampleChunk *data);

	protected:

		const ViSampleChunk *mData;

};

class ViDualObserver : public ViObserver
{

	public:

		ViDualObserver(QString name = "");
		virtual void setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData);

	protected:

		const ViSampleChunk *mData2;

};

class ViModifier : public ViProcessor
{

	public:

		ViModifier(QString name = "");
		virtual void setData(ViSampleChunk *data);

	protected:

		ViSampleChunk *mData;

};

#endif
