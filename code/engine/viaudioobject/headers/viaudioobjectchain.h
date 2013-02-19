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
		void executeNext();

	public:

		ViAudioObjectChain();
		ViAudioObjectChain(ViProject &project);

		void add(ViAudioObjectPointer object);
		void add(ViProject &project);

		void clear();

		void setFunction(ViFunctionCall function);
		void setFunction(QString function);

		void execute();

	private:

		QQueue<ViAudioObjectPointer> mObjects;
		ViAudioObjectPointer mCurrentObject;
		int mObjectCount;
		qreal mProgress;
		
		ViFunctionCall mFunction;

};

#endif
