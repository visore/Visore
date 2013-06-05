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
	emit progressed(mProgress + ((percentage / mObjectCount) * mCurrentWeight));
}

void ViAudioObjectChain::progressNext()
{
	mProgress += (100.0 / mObjectCount) * mCurrentWeight;
	emit progressed(mProgress);
	if(mCurrentFunctionIndex == 0)
	{
		QObject::disconnect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::disconnect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		QObject::disconnect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(progressNext()));
	}

	executeNext();
	if(mProgress == 100)
	{
		disconnect();
	}
}

void ViAudioObjectChain::executeNext()
{
	if(mObjects.isEmpty() && (mCurrentFunctionIndex == mFunctions.size() || mCurrentFunctionIndex == 0))
	{
		mProgress = 100;
		return;
	}
	else if(mCurrentFunctionIndex == 0)
	{
		mCurrentObject = mObjects.dequeue();
		QObject::connect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::connect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		QObject::connect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(progressNext()));
	}

	mCurrentFunction = mFunctions[mCurrentFunctionIndex];
	mCurrentWeight = mWeights[mCurrentFunctionIndex];
	++mCurrentFunctionIndex;
	if(mCurrentFunctionIndex >= mFunctions.size())
	{
		mCurrentFunctionIndex = 0;
	}

	mCurrentFunction.setObject(mCurrentObject.data());
	mCurrentFunction.execute();
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
	mFunctions.clear();
	mWeights.clear();
}

void ViAudioObjectChain::addFunction(ViFunctionCall function, qreal weight)
{
	mFunctions.append(function);
	if(weight > 0)
	{
		mWeights.append(weight);
	}
}

void ViAudioObjectChain::addFunction(QString function, qreal weight)
{
	function = function.replace("(", "");
	function = function.replace(")", "");
	function = function.replace(" ", "");
	addFunction(ViFunctionCall(function), weight);
}

void ViAudioObjectChain::execute()
{
	if(mWeights.size() != mFunctions.size())
	{
		mWeights.clear();
		qreal weight = 1.0 / mFunctions.size();
		for(int i = 0; i < mFunctions.size(); ++i)
		{
			mWeights.append(weight);
		}
	}

	emit progressed(0);
	mProgress = 0;
	mCurrentFunctionIndex = 0;
	mObjectCount = mObjects.size();
	executeNext();
}
