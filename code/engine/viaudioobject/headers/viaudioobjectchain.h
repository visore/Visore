#ifndef VIAUDIOOBJECTCHAIN_H
#define VIAUDIOOBJECTCHAIN_H

#include <viaudioobject.h>
#include <vifunctioncall.h>
#include <viproject.h>

class ViAudioObjectChain : public QObject
{

    Q_OBJECT

	signals:

		void progressed(qreal percentage);
		void statused(QString status);

	private slots:

		void progress(qreal percentage);
		void progressNext();
		void executeNext();

	public:

		ViAudioObjectChain();
		ViAudioObjectChain(ViProject &project);

		void add(ViAudioObjectPointer object);
		void add(ViAudioObjectQueue objects);
		void add(ViProject &project);

		void clear();

		void addFunction(ViFunctionCall function, qreal weight = -1);
		void addFunction(QString function, qreal weight = -1);

		void execute();

	private:

		QQueue<ViAudioObjectPointer> mObjects;
		ViAudioObjectPointer mCurrentObject;
		int mObjectCount;
		qreal mProgress;
		
		int mCurrentFunctionIndex;
		QQueue<ViFunctionCall> mFunctions;
		QQueue<qreal> mWeights;
		ViFunctionCall mCurrentFunction;
		qreal mCurrentWeight;

};

#endif
