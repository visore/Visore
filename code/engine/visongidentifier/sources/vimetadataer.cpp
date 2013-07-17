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
	mCurrentDescription = "";

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
	mMutex.lock();
	++mCurrentIdentifier;
	if(mCurrentIdentifier == mTotalIdentifiers)
	{
		mMutex.unlock();
		mMetadata = ViSongIdentifier::metadata(mIdentifiers);
		if(mMetadata.isValid())
		{
			if(!startNextRetriever())
			{
				finish();
			}
		}
		else
		{
			processNextBuffer();
		}
	}
	mMutex.unlock();


	/*if(!startNextIdentifier())
	{
		mMetadata = ViSongIdentifier::metadata(mIdentifiers);
		if(mMetadata.isValid())
		{
			LOG("Metadata detected as \"" + mMetadata.toShortString() + "\" (" + mCurrentDescription + ").");
			if(!startNextRetriever())
			{
				finish(true);
			}
		}
		else
		{
			LOG("No metadata detected (" + mCurrentDescription + ").");
			processNextBuffer();
		}
	}*/
}

void ViMetadataer::processRetrieval(bool success)
{
	if(success)
	{
		mMetadata = mRetrievers[mCurrentRetriever]->metadata();
		finish();
	}
	else if(!startNextRetriever())
	{
		finish();
	}
}

void ViMetadataer::enqueueBuffer(ViBuffer *buffer)
{
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, 0, buffer->size()), "Full track")); // Entire track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, buffer->size() * 0.2, buffer->size() * 0.8), "20% - 80% of track")); // From 20% to 80% of the track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, 0, buffer->size() / 2), "First half of track")); // First half of the track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, buffer->size() / 2, buffer->size()), "Second half of track")); // Second half of the track
}

bool ViMetadataer::startIdentifiers()
{
	mCurrentIdentifier = 0;
	mTotalIdentifiers = 0;

	QList<ViSongIdentifier*> identifiers;
	QString message = "";
	for(int i = 0; i < mIdentifiers.size(); ++i)
	{
		if(mIdentifiers[i]->networkError() == QNetworkReply::NoError)
		{
			identifiers.append(mIdentifiers[i]);
			if(message != "")
			{
				message += ", ";
			}
			message += mIdentifiers[i]->name();
		}
	}

	mTotalIdentifiers = identifiers.size();
	if(mTotalIdentifiers == 0)
	{
		LOG("No song identifiers available to detect the metadata.");
		return false;
	}

	LOG("Detecting the metadata: " + mCurrentDescription + " (" + message + ").");
	for(int i = 0; i < identifiers.size(); ++i)
	{
		identifiers[i]->identify(mCurrentBufferOffset);
	}
	return true;

	/*bool success = false;
	do
	{
		++mCurrentIdentifier;
		if(	mCurrentIdentifier < mIdentifiers.size() &&
			mIdentifiers[mCurrentIdentifier]->networkError() == QNetworkReply::NoError) // Connection error to the server
		{
			success = true;
			break;
		}
	}
	while(mCurrentIdentifier < mIdentifiers.size());

	if(success)
	{
		mIdentifiers[mCurrentIdentifier]->identify(mCurrentBufferOffset);
	}
	return success;*/
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
		enqueueBuffer(object->buffer(ViAudio::Target));
	}
	if(object->hasBuffer(ViAudio::Corrected))
	{
		enqueueBuffer(object->buffer(ViAudio::Corrected));
	}
	if(object->hasBuffer(ViAudio::Corrupted))
	{
		enqueueBuffer(object->buffer(ViAudio::Corrupted));
	}

	if(mBufferOffsets.isEmpty())
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
		enqueueBuffer(object->buffer(type));
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
	enqueueBuffer(buffer);

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
			enqueueBuffer(buffers[i]);
		}
	}

	if(mBufferOffsets.isEmpty())
	{
		LOG("Invalid buffer passed for the song identification.");
		return false;
	}

	processNextBuffer();
	return true;
}

void ViMetadataer::processNextBuffer()
{
	if(mBufferOffsets.isEmpty())
	{
		finish();
		return;
	}

	mTotalIdentifiers = 0;
	mCurrentIdentifier = -1;
	mCurrentRetriever = -1;

	QPair<ViBufferOffsets, QString> pair = mBufferOffsets.dequeue();
	mCurrentBufferOffset = pair.first;
	mCurrentDescription = pair.second;

	if(!startIdentifiers())
	{
		finish();
	}
}

void ViMetadataer::reset()
{
	for(int i = 0; i < mIdentifiers.size(); ++i)
	{
		mIdentifiers[i]->reset();
	}
	mDetected = false;
	mTotalIdentifiers = 0;
	mCurrentIdentifier = -1;
	mCurrentRetriever = -1;
	mCurrentBufferOffset.clear();
	mCurrentDescription = "";
	mBufferOffsets.clear();
	mMetadata.clear();
}

void ViMetadataer::finish()
{
	mDetected = mMetadata.isValid();
	if(mDetected)
	{
		if(mMetadata.hasCover())
		{
			LOG("The metadata and cover image was detected as \"" + mMetadata.toShortString() + "\".");
		}
		else
		{
			LOG("The metadata without the cover image was detected as \"" + mMetadata.toShortString() + "\".");
		}
	}
	else
	{
		LOG("No metadata detected.");
	}
	mBufferOffsets.clear();
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
