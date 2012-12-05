#ifndef VIAUDIOOBJECTQUEUE_H
#define VIAUDIOOBJECTQUEUE_H

#include "viaudioobject.h"
#include <QQueue>

class ViAudioObjectQueue : public QObject, public ViFunctor
{

    Q_OBJECT

	signals:

		void enqueued(ViAudioObjectPointer object);
		void dequeued(ViAudioObjectPointer object);
		void finished(ViAudioObjectPointer object);

	private slots:

		void finish();

	public:

		ViAudioObjectQueue();
		~ViAudioObjectQueue();

		void enqueue(ViAudioObjectPointer object);
		ViAudioObjectPointer dequeue();

		ViAudioObjectPointer current();
		void remove(ViAudioObject *object);

		int size();
		int isEmpty();
		void clear();

		ViAudioObjectPointer first();
		ViAudioObjectPointer last();

		ViAudioObjectPointer operator [] (int index);

	protected:

		void execute(ViFunctorParameter *data = NULL);

	private:

		QQueue<ViAudioObjectPointer> mQueue;

};

#endif
