#include <vimetadataer.h>
#include <viacoustididentifier.h>
#include <vienmfpidentifier.h>
#include <vimusicbrainzcoverretriever.h>

ViMetadataer::ViMetadataer()
	: QObject()
{
	mDetected = false;
	mCurrentIdentifier = 0;
	mCurrentRetriever = 0;
	mBuffer = NULL;

	mIdentifiers.append(new ViAcoustidIdentifier());
	mIdentifiers.append(new ViEnmfpIdentifier());

	mRetrievers.append(new ViMusicBrainzCoverRetriever());

	for(int i = 0; i < mIdentifiers.size(); ++i)
	{
		QObject::connect(mIdentifiers[i], SIGNAL(identified(bool)), this, SLOT(processIdentification(bool)), Qt::UniqueConnection);
	}
	for(int i = 0; i < mRetrievers.size(); ++i)
	{
		QObject::connect(mRetrievers[i], SIGNAL(retrieved(bool)), this, SLOT(processRetrieval(bool)), Qt::UniqueConnection);
	}
}

ViMetadataer::~ViMetadataer()
{
    viDeleteAll(mIdentifiers);
    viDeleteAll(mRetrievers);
}

void ViMetadataer::processIdentification(bool success)
{
	if(success)
	{
		mMetadata = mIdentifiers[mCurrentIdentifier]->metadata();
		if(!startNextRetriever())
		{
			finish(true);
		}
	}
	else if(!startNextIdentifier())
	{
		processNextBuffer();
	}
}

void ViMetadataer::processRetrieval(bool success)
{
	if(success)
	{
		mMetadata = mRetrievers[mCurrentRetriever]->metadata();
		finish(true);
	}
	else if(!startNextRetriever())
	{
		finish(true);
	}
}

bool ViMetadataer::startNextIdentifier()
{
	++mCurrentIdentifier;
	if(mCurrentIdentifier >= mIdentifiers.size())
	{
		return false;
	}
	mIdentifiers[mCurrentIdentifier]->identify(mBuffer);
	return true;
}

bool ViMetadataer::startNextRetriever()
{
	++mCurrentRetriever;
	if(mCurrentRetriever >= mRetrievers.size())
	{
		return false;
	}
	mRetrievers[mCurrentRetriever]->retrieve(mMetadata);
	return true;
}

bool ViMetadataer::detect(ViAudioObjectPointer object)
{
	reset();

	if(object->hasBuffer(ViAudio::Target))
	{
		mBuffers.enqueue(object->buffer(ViAudio::Target));
	}
	if(object->hasBuffer(ViAudio::Corrected))
	{
		mBuffers.enqueue(object->buffer(ViAudio::Corrected));
	}
	if(object->hasBuffer(ViAudio::Corrupted))
	{
		mBuffers.enqueue(object->buffer(ViAudio::Corrupted));
	}

	if(mBuffers.isEmpty())
	{
		LOG("Invalid buffer passed for the song identification.");
		return false;
	}

	processNextBuffer();
	return true;
}

bool ViMetadataer::detect(ViAudioObjectPointer object, ViAudio::Type type)
{
	reset();

	if(object->hasBuffer(type))
	{
		mBuffers.enqueue(object->buffer(type));
	}
	else
	{
		LOG("Invalid buffer passed for the song identification.");
		return false;
	}

	processNextBuffer();
	return true;
}

bool ViMetadataer::detect(ViBuffer *buffer)
{
	reset();

	if(buffer == NULL)
	{
		LOG("Invalid buffer passed for the song identification.");
		return false;
	}
	mBuffers.enqueue(buffer);

	processNextBuffer();
	return true;
}

bool ViMetadataer::detect(QList<ViBuffer*> buffers)
{
	reset();

	for(int i = 0; i < buffers.size(); ++i)
	{
		if(buffers[i] != NULL)
		{
			mBuffers.enqueue(buffers[i]);
		}
	}

	if(mBuffers.isEmpty())
	{
		LOG("Invalid buffer passed for the song identification.");
		return false;
	}

	processNextBuffer();
	return true;
}

void ViMetadataer::processNextBuffer()
{
	if(mBuffers.isEmpty())
	{
		finish(false);
		return;
	}
	mBuffer = mBuffers.dequeue();
	startNextIdentifier();
}

void ViMetadataer::reset()
{
	mDetected = false;
	mCurrentIdentifier = -1;
	mCurrentRetriever = -1;
	mBuffer = NULL;
	mBuffers.clear();
	mMetadata = ViMetadata();
}

void ViMetadataer::finish(bool success)
{
	mDetected = success;
	if(mDetected)
	{
		mBuffers.clear();
	}
	emit finished(mDetected);
}

bool ViMetadataer::detected()
{
	return mDetected;
}

ViMetadata ViMetadataer::metadata()
{
	return mMetadata;
}
