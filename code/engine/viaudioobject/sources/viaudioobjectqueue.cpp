#include "viaudioobjectqueue.h"
#include "vilogger.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

void d(ViAudioObject *object)
{

}

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
	boost::function<void(ViAudioObject*)> f = bind(&ViAudioObjectQueue::remove, this, _1);
	void *ff = f;
	object.setDeleter(ViFunctorObject<ViAudioObjectQueue, ViAudioObject>(this, &ViAudioObjectQueue::remove));
	//object.setUnusedLimit(1);
	mQueue.enqueue(object);
	QObject::connect(object.data(), SIGNAL(finished()), this, SLOT(finish()), Qt::UniqueConnection);
	//QObject::connect(&object, SIGNAL(unused()), this, SLOT(remove()), Qt::UniqueConnection);
	emit enqueued(object);
}

ViAudioObjectPointer ViAudioObjectQueue::dequeue()
{
	ViAudioObjectPointer object = mQueue.dequeue();
	QObject::disconnect(object.data(), SIGNAL(finished()), this, SLOT(finish()));
	//QObject::disconnect(&object, SIGNAL(unused()), this, SLOT(remove()));
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
{LOG("zxxxx11");
	/*for(int i = 0; i < size(); ++i)
	{
		if(object == mQueue[i])
		{
			QObject::disconnect(mQueue[i].data(), SIGNAL(finished()), this, SLOT(finish()));
			QObject::disconnect(&mQueue[i], SIGNAL(unused()), this, SLOT(remove()));
			mQueue.removeAt(i);
			return;
		}
	}*/
	/*for(int i = 0; i < size(); ++i)
	{
		if(object == mQueue[i].data())
		{
			QObject::disconnect(mQueue[i].data(), SIGNAL(finished()), this, SLOT(finish()));
			//QObject::disconnect(&mQueue[i], SIGNAL(unused()), this, SLOT(remove()));
			mQueue.removeAt(i);
			return;
		}
	}*/
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
