#include <viaudioobjectchain.h>

ViAudioObjectChain::ViAudioObjectChain()
	: QObject()
{
}

ViAudioObjectChain::ViAudioObjectChain(ViProject &project)
	: QObject()
{
	add(project);
}

void ViAudioObjectChain::progress(qreal percentage)
{
	emit progressed(mProgress + (percentage / mObjectCount));
	if(percentage >= 100)
	{
		mProgress += 100.0 / mObjectCount;
		QObject::disconnect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::disconnect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		executeNext();
	}
	if(mProgress == 100)
	{
		disconnect();
	}
}

void ViAudioObjectChain::executeNext()
{
	if(!mObjects.isEmpty())
	{
		mCurrentObject = mObjects.dequeue();
		mFunction.setObject(mCurrentObject.data());
		QObject::connect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::connect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		mFunction.execute();
	}
}

void ViAudioObjectChain::add(ViAudioObjectPointer object)
{
	mObjects.append(object);
}

void ViAudioObjectChain::add(ViAudioObjectQueue objects)
{
	mObjects.append(objects);
}

void ViAudioObjectChain::add(ViProject &project)
{
	mObjects.append(project.objectQueue());
}

void ViAudioObjectChain::clear()
{
	mObjects.clear();
}

void ViAudioObjectChain::setFunction(ViFunctionCall function)
{
	mFunction = function;
}

void ViAudioObjectChain::setFunction(QString function)
{
	function = function.replace("(", "");
	function = function.replace(")", "");
	function = function.replace(" ", "");
	setFunction(ViFunctionCall(function));
}

void ViAudioObjectChain::execute()
{
	mProgress = 0;
	mObjectCount = mObjects.size();
	executeNext();
}
