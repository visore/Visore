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
    if(mCurrentIndex == 0)
	{
		QObject::disconnect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::disconnect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		QObject::disconnect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(progressNext()));
	}
	executeNext();
}

void ViAudioObjectChain::executeNext()
{
    if(mObjects.isEmpty() && (mCurrentIndex == mFunctions.size() || mCurrentIndex == 0))
	{
		clear();
        mProgress = 100;
        emit finished();
        disconnect();

		return;
	}
    else if(mCurrentIndex == 0)
    {
		mCurrentObject = mObjects.dequeue();
		QObject::connect(mCurrentObject.data(), SIGNAL(statused(QString)), this, SIGNAL(statused(QString)));
		QObject::connect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
		QObject::connect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(progressNext()));
	}

    bool wait = mWait[mCurrentIndex];
    mCurrentFunction = mFunctions[mCurrentIndex];
    mCurrentWeight = mWeights[mCurrentIndex];
    ++mCurrentIndex;
    if(mCurrentIndex >= mFunctions.size())
	{
        mCurrentIndex = 0;
	}

	mCurrentFunction.setObject(mCurrentObject.data());
    mCurrentFunction.execute();
    if(!wait)
    {
        progressNext();
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
	mObjects.append(project.objects());
}

void ViAudioObjectChain::clear()
{
	mCurrentObject.setNull();
	mObjects.clear();
	mFunctions.clear();
	mWeights.clear();
    mWait.clear();
}

void ViAudioObjectChain::addFunction(ViFunctionCall function, qreal weight, bool wait)
{
	mFunctions.append(function);
	if(weight > 0)
	{
		mWeights.append(weight);
        mWait.append(wait);
	}
}

void ViAudioObjectChain::addFunction(QString function, qreal weight, bool wait)
{
	function = function.replace("(", "");
	function = function.replace(")", "");
	function = function.replace(" ", "");
    addFunction(ViFunctionCall(function), weight, wait);
}

void ViAudioObjectChain::execute(QString status)
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
	emit statused(status);
	mProgress = 0;
    mCurrentIndex = 0;
	mObjectCount = mObjects.size();
	executeNext();
}
