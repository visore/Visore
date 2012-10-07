#include "viaudioobjectqueue.h"

ViAudioObjectQueue::ViAudioObjectQueue()
	: QObject(),
	  QQueue()
{
}

void ViAudioObjectQueue::enqueue(ViAudioObject *object)
{
	QQueue::enqueue(object);
	QObject::connect(object, SIGNAL(finished()), this, SLOT(finish()), Qt::UniqueConnection);
	emit enqueued(object);
}

ViAudioObject* ViAudioObjectQueue::dequeue()
{
	ViAudioObject *object = QQueue::dequeue();
	QObject::disconnect(object, SIGNAL(finished()), this, SLOT(finish()));
	emit dequeued(object);
	return object;
}

ViAudioObject* ViAudioObjectQueue::current()
{
	if(isEmpty())
	{
		return NULL;
	}
	return last();
}

void ViAudioObjectQueue::finish()
{
	emit finished((ViAudioObject*) sender());
}
