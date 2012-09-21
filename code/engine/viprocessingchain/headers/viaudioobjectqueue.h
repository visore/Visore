#ifndef VIAUDIOOBJECTQUEUE_H
#define VIAUDIOOBJECTQUEUE_H

#include "viaudioobject.h"
#include <QQueue>

class ViAudioObjectQueue : public QObject, public QQueue<ViAudioObject*>
{

    Q_OBJECT

	signals:

		void enqueued(ViAudioObject *object);
		void dequeued(ViAudioObject *object);
		void finished(ViAudioObject *object);

	private slots:

		void finish();

	public:

		ViAudioObjectQueue();
		void enqueue(ViAudioObject *object);
		ViAudioObject* dequeue();
		ViAudioObject* current();

};

#endif
