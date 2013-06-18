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

        //If wait is true, waiting for ViAudioObject to emit finished()
        //If wait is false, the function will be called and continue without waiting for the finished signal
        void addFunction(ViFunctionCall function, qreal weight = -1, bool wait = true);
        void addFunction(QString function, qreal weight = -1, bool wait = true);

		void execute();

	private:

		QQueue<ViAudioObjectPointer> mObjects;
		ViAudioObjectPointer mCurrentObject;
		int mObjectCount;
		qreal mProgress;
		
        int mCurrentIndex;
		QQueue<ViFunctionCall> mFunctions;
        QQueue<qreal> mWeights;
        QQueue<bool> mWait;
		ViFunctionCall mCurrentFunction;
		qreal mCurrentWeight;

};

#endif
