#include "viaudioobjectqueue.h"
#include "vilogger.h"

ViAudioObjectQueue::ViAudioObjectQueue()
	: QObject()
{
}

ViAudioObjectQueue::~ViAudioObjectQueue()
{
	clear();
}

void ViAudioObjectQueue::enqueue(ViAudioObjectPointer object)
{
	object.setUnusedLimit(1);
	object.setDeleter(this);
	QObject::connect(object.data(), SIGNAL(finished()), this, SLOT(finish()), Qt::UniqueConnection);
	mQueue.enqueue(object);
	emit enqueued(object);

cout<<"qqq: ";
for(int i = 0; i < size(); ++i)
{
cout<<mQueue[i].referenceCount()<<"  ";
}cout<<endl;
}

ViAudioObjectPointer ViAudioObjectQueue::dequeue()
{
	ViAudioObjectPointer object = mQueue.dequeue();
	QObject::disconnect(object.data(), SIGNAL(finished()), this, SLOT(finish()));
	emit dequeued(object);
	return object;
}

ViAudioObjectPointer ViAudioObjectQueue::current()
{
	if(isEmpty())
	{
		return ViAudioObject::createNull();
	}
	return mQueue.last();
}

void ViAudioObjectQueue::finish()
{
	emit finished(ViAudioObject::create((ViAudioObject*) sender()));
}

void ViAudioObjectQueue::remove(ViAudioObject *object)
{
	for(int i = 0; i < size(); ++i)
	{
		if(object == mQueue[i].data())
		{
			QObject::disconnect(mQueue[i].data(), SIGNAL(finished()), this, SLOT(finish()));
			mQueue.removeAt(i);
			break;
		}
	}
}

int ViAudioObjectQueue::size()
{
	return mQueue.size();
}

int ViAudioObjectQueue::isEmpty()
{
	return mQueue.isEmpty();
}

void ViAudioObjectQueue::clear()
{
	mQueue.clear();
}

ViAudioObjectPointer ViAudioObjectQueue::first()
{
	return	mQueue.first();
}

ViAudioObjectPointer ViAudioObjectQueue::last()
{
	return mQueue.last();
}

void ViAudioObjectQueue::execute(ViFunctorParameter *data)
{
	ViAudioObject *object = static_cast<ViAudioObject*>(data);
	if(object != NULL)
	{
		remove(object);
	}
}
