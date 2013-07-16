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

	//mIdentifiers.append(new ViAcoustidIdentifier());
	//mIdentifiers.append(new ViEnmfpIdentifier());

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
	if(!startNextIdentifier())
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
	}
}

void ViMetadataer::processRetrieval(bool success)
{
	if(success)
	{
		mMetadata = mRetrievers[mCurrentRetriever]->metadata();
		LOG("Cover image found for \"" + mMetadata.toShortString() + "\".");
		finish(true);
	}
	else if(!startNextRetriever())
	{
		LOG("No cover image found for \"" + mMetadata.toShortString() + "\".");
		finish(true);
	}
}

void ViMetadataer::enqueueBuffer(ViBuffer *buffer)
{
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, 0, buffer->size()), "Full track")); // Entire track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, buffer->size() * 0.2, buffer->size() * 0.8), "20% - 80% of track")); // From 20% to 80% of the track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, 0, buffer->size() / 2), "First half of track")); // First half of the track
	mBufferOffsets.enqueue(QPair<ViBufferOffsets, QString>(ViBufferOffsets(buffer, buffer->size() / 2, buffer->size()), "Second half of track")); // Second half of the track
}

bool ViMetadataer::startNextIdentifier()
{
	++mCurrentIdentifier;
	if(mCurrentIdentifier >= mIdentifiers.size())
	{
		return false;
	}
	mIdentifiers[mCurrentIdentifier]->identify(mCurrentBufferOffset);
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
		finish(false);
		return;
	}
	mCurrentIdentifier = -1;
	mCurrentRetriever = -1;
	QPair<ViBufferOffsets, QString> pair = mBufferOffsets.dequeue();
	mCurrentBufferOffset = pair.first;
	mCurrentDescription = pair.second;
	if(!startNextIdentifier())
	{
		finish(false);
	}
}

void ViMetadataer::reset()
{
	mDetected = false;
	mCurrentIdentifier = -1;
	mCurrentRetriever = -1;
	mCurrentBufferOffset.clear();
	mCurrentDescription = "";
	mBufferOffsets.clear();
	mMetadata = ViMetadata();
}

void ViMetadataer::finish(bool success)
{
	mDetected = success;
	if(mDetected)
	{
		mBufferOffsets.clear();
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
