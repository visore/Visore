#include <viaudioobject.h>
#include <viwaveformer.h>
#include <vimetadataer.h>
#include <vifouriercrossaligner.h>
#include <viacoustididentifier.h>
#include <vienmfpidentifier.h>
#include <vimanager.h>
#include <viaudiocodec.h>
#include <vicorrelator.h>
#include <QSet>

/*******************************************************************************************************************

	CONSTRUCTORS, DESTRUCTORS & GENERAL

*******************************************************************************************************************/

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject(), ViId()
{
	setAutoDestruct(autoDestruct);
	mIsSong = false;

	mTargetBuffer = NULL;
	mCorruptedBuffer = NULL;
	mCorrectedBuffer = NULL;
	mTemporaryBuffer = NULL;

	mTargetFile = "";
	mCorruptedFile = "";
	mCorrectedFile = "";

	mEncoder = NULL;
	mDecoder = NULL;
	mAligner = NULL;
	mWaveFormer = NULL;
	mMetadataer = NULL;
	mCorrector = NULL;

	mProgressParts = 1;
	mProgress = 0;
	mIsBusy = false;

	mSideNumber = 0;
	mTrackNumber = 0;

	setAligner(new ViFourierCrossAligner());
}

ViAudioObject::~ViAudioObject()
{
    clearBuffers(mDestructType);

    viDeleteAll(mWaveForms);

	if(mEncoder != NULL)
	{
		delete mEncoder;
		mEncoder = NULL;
	}
	if(mDecoder != NULL)
	{
		delete mDecoder;
		mDecoder = NULL;
	}

	if(mAligner != NULL)
	{
		delete mAligner;
		mAligner = NULL;
	}

	if(mWaveFormer != NULL)
	{
		delete mWaveFormer;
		mWaveFormer = NULL;
	}

	if(mMetadataer != NULL)
	{
		delete mMetadataer;
		mMetadataer = NULL;
	}

	if(mCorrector != NULL)
	{
		delete mCorrector;
		mCorrector = NULL;
    }

    clearCorrelators();
}

ViAudioObjectPointer ViAudioObject::create(ViAudioObject *object)
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(object);
	pointer->thisPointer = pointer;
    pointer.setUnusedLimit(1);
	return pointer;
}

ViAudioObjectPointer ViAudioObject::create(bool autoDestruct)
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(new ViAudioObject(autoDestruct));
	pointer->thisPointer = pointer;
    pointer.setUnusedLimit(1);
	return pointer;
}

ViAudioObjectPointer ViAudioObject::createNull()
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(NULL);
	pointer->thisPointer = pointer;
    pointer.setUnusedLimit(1);
	return pointer;
}

QQueue<ViAudio::Type> ViAudioObject::decomposeTypes(ViAudio::Type type, ViAudio::Type exclude)
{
	QQueue<ViAudio::Type> result;
	if(type & ViAudio::Target && !(exclude & ViAudio::Target))
	{
		result.enqueue(ViAudio::Target);
	}
	if(type & ViAudio::Corrupted && !(exclude & ViAudio::Corrupted))
	{
		result.enqueue(ViAudio::Corrupted);
	}
	if(type & ViAudio::Corrected && !(exclude & ViAudio::Corrected))
	{
		result.enqueue(ViAudio::Corrected);
	}
	if(type & ViAudio::Temporary && !(exclude & ViAudio::Temporary))
	{
		result.enqueue(ViAudio::Temporary);
	}
	return result;
}

ViAudio::Type ViAudioObject::composeTypes(QQueue<ViAudio::Type> &types, ViAudio::Type exclude)
{
	int result = 0;
	for(int i = 0; i < types.size(); ++i)
	{
		if(!(types[i] & exclude))
		{
			result |= types[i];
		}
	}
	return (ViAudio::Type) result;
}

bool ViAudioObject::hasResource(ViAudio::Type type)
{
	return (filePath(type) != "") || (buffer(type, true) != NULL);
}

ViAudioObject::Resource ViAudioObject::resourceAvailable(ViAudio::Type type)
{
	bool fileAvailable = (filePath(type) != "");
	bool bufferAvailable = (buffer(type, true) != NULL);

	if(fileAvailable && bufferAvailable)
	{
		return ViAudioObject::Both;
	}
	else if(fileAvailable)
	{
		return ViAudioObject::File;
	}
	else if(bufferAvailable)
	{
		return ViAudioObject::Buffer;
	}
	return ViAudioObject::None;
}

ViAudio::Type ViAudioObject::availableResources(ViAudioObject::Resource resource)
{
	int result = 0;
	if(resourceAvailable(ViAudio::Target) & resource)
	{
		result |= ViAudio::Target;
	}
	if(resourceAvailable(ViAudio::Corrupted) & resource)
	{
		result |= ViAudio::Corrupted;
	}
	if(resourceAvailable(ViAudio::Corrected) & resource)
	{
		result |= ViAudio::Corrected;
	}
	if(result == 0)
	{
		return ViAudio::Undefined;
	}
	return (ViAudio::Type) result;
}

qreal ViAudioObject::length(ViAudioPosition::Unit unit)
{
	qreal maximum = 0;
	if(hasBuffer(ViAudio::Target) && buffer(ViAudio::Target)->size() > maximum)
	{
		maximum = length(ViAudio::Target, unit);
	}
	if(hasBuffer(ViAudio::Corrupted) && buffer(ViAudio::Corrupted)->size() > maximum)
	{
		maximum = length(ViAudio::Corrupted, unit);
	}
	if(hasBuffer(ViAudio::Corrected) && buffer(ViAudio::Corrected)->size() > maximum)
	{
		maximum = length(ViAudio::Corrected, unit);
	}
	return maximum;
}

qreal ViAudioObject::length(ViAudio::Type type, ViAudioPosition::Unit unit)
{
	if(hasBuffer(type))
	{
		return ViAudioPosition::convertPosition(buffer(type)->size(), ViAudioPosition::Bytes, unit, buffer(type)->format());
	}
	return 0;
}

/*******************************************************************************************************************

	AUTO DESTRUCT

*******************************************************************************************************************/

void ViAudioObject::setAutoDestruct(bool destruct)
{
	QMutexLocker locker(&mMutex);
	if(destruct)
	{
		mDestructType = ViAudio::All;
	}
	else
	{
		mDestructType = ViAudio::Undefined;
	}
}

void ViAudioObject::addDestructRule(ViAudio::Type type, bool destruct)
{
	QMutexLocker locker(&mMutex);
	QSet<ViAudio::Type> set;
	QSet<ViAudio::Type> values;
	values.insert(ViAudio::Target);
	values.insert(ViAudio::Corrupted);
	values.insert(ViAudio::Corrected);
	values.insert(ViAudio::Temporary);

	//Old Values
	foreach(const ViAudio::Type &value, values)
	{
		if(mDestructType & value)
		{
			set.insert(value);
		}
	}

	//New Values
	foreach(const ViAudio::Type &value, values)
	{
		if(type & value)
		{
			if(destruct)
			{
				set.insert(value);
			}
			else
			{
				set.remove(value);
			}
		}
	}

	if(set.isEmpty())
	{
		mDestructType = ViAudio::Undefined;
	}
	else
	{
		QList<ViAudio::Type> list = set.toList();
		int temp = list[0];
        for(int i = 1; i < list.size(); ++i)
        {
			temp |= list[i];
		}
		mDestructType = (ViAudio::Type) temp;
	}
}

/*******************************************************************************************************************

	FORMATS

*******************************************************************************************************************/

void ViAudioObject::setFormat(ViAudio::Type type, ViAudioFormat format)
{
    if(type == ViAudio::Target)
    {
        return setTargetFormat(format);
    }
    else if(type == ViAudio::Corrupted)
    {
        return setCorruptedFormat(format);
    }
    else if(type == ViAudio::Corrected)
    {
        return setCorrectedFormat(format);
    }
}

void ViAudioObject::setTargetFormat(ViAudioFormat format)
{
    mTargetFormat = format;
}

void ViAudioObject::setCorruptedFormat(ViAudioFormat format)
{
    mCorruptedFormat = format;
}

void ViAudioObject::setCorrectedFormat(ViAudioFormat format)
{
    mCorrectedFormat = format;
}

ViAudioFormat ViAudioObject::format(ViAudio::Type type)
{
	if(type == ViAudio::Target)
	{
		return targetFormat();
	}
	else if(type == ViAudio::Corrupted)
	{
		return corruptedFormat();
	}
	else if(type == ViAudio::Corrected)
	{
		return correctedFormat();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::targetFormat()
{
    return mTargetFormat;
}

ViAudioFormat ViAudioObject::corruptedFormat()
{
    return mCorruptedFormat;
}

ViAudioFormat ViAudioObject::correctedFormat()
{
    return mCorrectedFormat;
}

/*******************************************************************************************************************

	ENCODE & DECODE

*******************************************************************************************************************/

void ViAudioObject::setEncoder(ViAudioCoder *coder)
{
	if(mEncoder != NULL)
	{
		delete mEncoder;
	}
	mEncoder = coder;
	QObject::connect(mEncoder, SIGNAL(finished()), this, SLOT(encodeNext()));
	QObject::connect(mEncoder, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

bool ViAudioObject::hasEncoder()
{
	return mEncoder != NULL;
}

bool ViAudioObject::encode(int type)
{
    return encode((ViAudio::Type) type);
}

bool ViAudioObject::encode(ViAudio::Type type, bool clearWhenFinished)
{
	setStarted();
	QMutexLocker locker(&mMutex);
	mClearEncodedBuffer = clearWhenFinished;
	mPreviousEncodedType = ViAudio::Undefined;
	mCodingInstructions = decomposeTypes(type);

	QString thePath = "";
	ViBuffer *theBuffer = NULL;
	for(int i = 0; i < mCodingInstructions.size(); ++i)
	{
		locker.unlock();
		if(!hasBuffer(mCodingInstructions[i]))
		{
			locker.relock();
			mCodingInstructions.removeAt(i);
			--i;
			locker.unlock();
		}
		else if(!hasFile(mCodingInstructions[i]))
		{
			setFilePath(mCodingInstructions[i], temporaryFilePath(mCodingInstructions[i]));
		}
		locker.relock();
	}
	if(mCodingInstructions.isEmpty())
	{
		mCodingInstructions.clear();
		locker.unlock();
		emit encoded();
		setFinished();
		return false;
	}

	setEncoder(new ViAudioCoder());
	
	setProgress(mCodingInstructions.size());
	locker.unlock();
	logStatus("Encoding track.");
	encodeNext();
	return true;
}

void ViAudioObject::encodeNext()
{
	QMutexLocker locker(&mMutex);
	if(mClearEncodedBuffer)
    {
		locker.unlock();
		clearBuffer(mPreviousEncodedType);
		locker.relock();
		mPreviousEncodedType = ViAudio::Undefined;
	}

	if(mCodingInstructions.isEmpty())
	{
		log("Buffers encoded.");
		delete mEncoder;
		mEncoder = NULL;
		locker.unlock();
		emit encoded();
        emit changed();
		setFinished();
	}
	else
	{
		mPreviousEncodedType = mCodingInstructions.dequeue();
		locker.unlock();
		
		QString thePath = filePath(mPreviousEncodedType);
		ViBuffer *theBuffer = buffer(mPreviousEncodedType, true);
		locker.relock();
		mEncoder->encode(theBuffer, thePath, theBuffer->format(), mSongInfo);
	}
}

void ViAudioObject::setDecoder(ViAudioCoder *coder)
{
	if(mDecoder != NULL)
	{
		delete mDecoder;
	}
	mDecoder = coder;
	QObject::connect(mDecoder, SIGNAL(finished()), this, SLOT(decodeNext()));
	QObject::connect(mDecoder, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

bool ViAudioObject::hasDecoder()
{
	return mDecoder != NULL;
}

bool ViAudioObject::decode(int type)
{
    return decode((ViAudio::Type) type);
}

bool ViAudioObject::decode(ViAudio::Type type)
{
	setStarted();
	QMutexLocker locker(&mMutex);
	mCodingInstructions = decomposeTypes(type);

	for(int i = 0; i < mCodingInstructions.size(); ++i)
	{
		ViAudio::Type type = mCodingInstructions[i];
		locker.unlock();
		if(hasBuffer(type) || !hasFile(type))
		{
			locker.relock();
			mCodingInstructions.removeAt(i);
			locker.unlock();
			--i;
		}
		locker.relock();
	}
	if(mCodingInstructions.isEmpty())
	{
		log("No files were decoded.");
		mCodingInstructions.clear();
		locker.unlock();
		emit decoded();
		setFinished();
		return false;
	}

	setDecoder(new ViAudioCoder());
	
	setProgress(mCodingInstructions.size());
	locker.unlock();
	logStatus("Decoding track.");
	decodeNext();
	return true;
}

void ViAudioObject::decodeNext()
{
	QMutexLocker locker(&mMutex);
	if(mCodingInstructions.isEmpty())
	{
		log("Files decoded.");
		delete mDecoder;
		mDecoder = NULL;
		locker.unlock();
		emit decoded();
		setFinished();
	}
	else
	{
		ViAudio::Type type = mCodingInstructions.dequeue();
		locker.unlock();
		QString thePath = filePath(type);
		clearBuffer(type);
		ViBuffer *theBuffer = buffer(type);
		locker.relock();
		mDecoder->decode(thePath, theBuffer);
	}
}

/*******************************************************************************************************************

	ALIGN

*******************************************************************************************************************/

void ViAudioObject::setAligner(ViAligner *aligner)
{
	if(mAligner != NULL)
	{
		delete mAligner;
	}
	mAligner = aligner;
	QObject::connect(mAligner, SIGNAL(finished()), this, SLOT(alignNext()));
	QObject::connect(mAligner, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

bool ViAudioObject::hasAligner()
{
	return mAligner != NULL;
}

bool ViAudioObject::align(ViAligner *aligner)
{
	setStarted();
	if(aligner != NULL)
	{
		setAligner(aligner);
	}
	if(!hasAligner())
	{
		log("No aligner was specified.");
		emit aligned();
		setFinished();
		return false;
	}

	mAlignerTypes = availableResources(ViAudioObject::Buffer);
	mAlignerInstructions = decomposeTypes(mAlignerTypes);
	if(mAlignerInstructions.size() < 2)
	{
		log("At least two buffers are needed for alignment.");
		emit aligned();
		setFinished();
		return false;
	}

	logStatus("Aligning track.");
	mMainAligner = mAlignerInstructions.dequeue();
	alignNext();
	return false;
}

void ViAudioObject::alignNext()
{
	if(mAlignerInstructions.isEmpty())
	{
		log("Tracks aligned.");
		emit aligned();
		setFinished();
	}
	else
	{
		mAligner->align(buffer(mMainAligner), buffer(mAlignerInstructions.dequeue()));
	}
}

/*******************************************************************************************************************

	LOGGING

*******************************************************************************************************************/

void ViAudioObject::log(QString message, QtMsgType type)
{
	if(!message.endsWith("."))
	{
		message += ".";
	}
	LOG(message, type);
}

void ViAudioObject::logStatus(QString message, QtMsgType type)
{
	log(message, type);
	if(message.endsWith("."))
	{
		message = message.remove(message.size() - 1, 1);
	}
	emit statused(message);
}

/*******************************************************************************************************************

	PROGRESS

*******************************************************************************************************************/

void ViAudioObject::setFinished()
{
	mIsBusy = false;
	emit progressed(100);
	emit finished();
}

void ViAudioObject::setStarted()
{
	mIsBusy = true;
	emit progressed(0);
	emit started();
}

bool ViAudioObject::isFinished()
{
	return !mIsBusy;
}

bool ViAudioObject::isBusy()
{
	return mIsBusy;
}

void ViAudioObject::progress(qreal progress)
{
	qreal pro = (progress / mProgressParts);
	emit progressed(mProgress + pro);
	if(progress >= 100)
	{
		mProgress += pro;
	}
	if(mProgress >= 100)
	{
		mProgress = 0;
		mProgressParts = 1;
		emit progressed(100);
	}
}

void ViAudioObject::setProgress(qreal parts)
{
    mProgress = 0;
	mProgressParts = parts;
}

/*******************************************************************************************************************

	BUFFERS

*******************************************************************************************************************/

void ViAudioObject::transferBuffer(ViAudioObjectPointer object, ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	object->addDestructRule(type, false);
	locker.unlock();
	setBuffer(type, object->buffer(type));
	setFilePath(type, object->filePath(type));
}

ViBuffer* ViAudioObject::buffer(ViAudio::Type type, bool dontCreate)
{
	if(type == ViAudio::Target)
	{
		return targetBuffer(dontCreate);
	}
	else if(type == ViAudio::Corrupted)
	{
		return corruptedBuffer(dontCreate);
	}
	else if(type == ViAudio::Corrected)
	{
		return correctedBuffer(dontCreate);
	}
	else if(type == ViAudio::Temporary)
	{
		return temporaryBuffer(dontCreate);
	}
	return NULL;
}

ViBuffer* ViAudioObject::targetBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mTargetBuffer == NULL)
	{
		mTargetBuffer = new ViBuffer();
        QObject::connect(mTargetBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setTargetFormat(ViAudioFormat)));
	}
	return mTargetBuffer;
}

ViBuffer* ViAudioObject::corruptedBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mCorruptedBuffer == NULL)
	{
		mCorruptedBuffer = new ViBuffer();
        QObject::connect(mCorruptedBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setCorruptedFormat(ViAudioFormat)));
	}
	return mCorruptedBuffer;
}

ViBuffer* ViAudioObject::correctedBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mCorrectedBuffer == NULL)
	{
		mCorrectedBuffer = new ViBuffer();
        QObject::connect(mCorrectedBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setCorrectedFormat(ViAudioFormat)));
	}
	return mCorrectedBuffer;
}

ViBuffer* ViAudioObject::temporaryBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mTemporaryBuffer == NULL)
	{
		mTemporaryBuffer = new ViBuffer();
	}
	return mTemporaryBuffer;
}

void ViAudioObject::setBuffer(ViAudio::Type type, ViBuffer *buffer)
{
	if(type == ViAudio::Target)
	{
		setTargetBuffer(buffer);
	}
	else if(type == ViAudio::Corrupted)
	{
		setCorruptedBuffer(buffer);
	}
	else if(type == ViAudio::Corrected)
	{
		setCorrectedBuffer(buffer);
	}
}

void ViAudioObject::setTargetBuffer(ViBuffer *buffer)
{
	clearTargetBuffer();
	QMutexLocker locker(&mMutex);
	mTargetBuffer = buffer;
    if(buffer->format().isValid())
    {
        setTargetFormat(buffer->format());
    }
    QObject::connect(mTargetBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setTargetFormat(ViAudioFormat)));
}

void ViAudioObject::setCorruptedBuffer(ViBuffer *buffer)
{
	clearCorruptedBuffer();
	QMutexLocker locker(&mMutex);
	mCorruptedBuffer = buffer;
    if(buffer->format().isValid())
    {
        setCorruptedFormat(buffer->format());
    }
    QObject::connect(mCorruptedBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setCorruptedFormat(ViAudioFormat)));
}

void ViAudioObject::setCorrectedBuffer(ViBuffer *buffer)
{
	clearCorrectedBuffer();
	QMutexLocker locker(&mMutex);
	mCorrectedBuffer = buffer;
    if(buffer->format().isValid())
    {
        setCorrectedFormat(buffer->format());
    }
    QObject::connect(mCorrectedBuffer, SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setCorrectedFormat(ViAudioFormat)));
}

void ViAudioObject::clearBuffers(ViAudio::Type type)
{
	if(type & ViAudio::Target)
    {
		clearTargetBuffer();
	}
	if(type & ViAudio::Corrupted)
    {
		clearCorruptedBuffer();
	}
	if(type & ViAudio::Corrected)
    {
		clearCorrectedBuffer();
	}
	if(type & ViAudio::Temporary)
	{
		clearTemporaryBuffer();
	}
}

void ViAudioObject::clearBuffer(ViAudio::Type type)
{
	if(type == ViAudio::Target)
	{
		clearTargetBuffer();
	}
	else if(type == ViAudio::Corrupted)
	{
		clearCorruptedBuffer();
	}
	else if(type == ViAudio::Corrected)
	{
		clearCorrectedBuffer();
	}
	else if(type == ViAudio::Temporary)
	{
		clearTemporaryBuffer();
	}
}

void ViAudioObject::clearTargetBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTargetBuffer != NULL)
	{
		delete mTargetBuffer;
		mTargetBuffer = NULL;
	}
}

void ViAudioObject::clearCorruptedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorruptedBuffer != NULL)
	{
		delete mCorruptedBuffer;
		mCorruptedBuffer = NULL;
	}
}

void ViAudioObject::clearCorrectedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorrectedBuffer != NULL)
	{
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}

void ViAudioObject::clearTemporaryBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTemporaryBuffer != NULL)
	{
		delete mTemporaryBuffer;
		mTemporaryBuffer = NULL;
	}
}

bool ViAudioObject::hasBuffer(ViAudio::Type type)
{
	return buffer(type, true) != NULL;
}

/*******************************************************************************************************************

	FILES

*******************************************************************************************************************/

QString ViAudioObject::filePath(ViAudio::Type type)
{
	if(type == ViAudio::Target)
	{
		return targetFilePath();
	}
	else if(type == ViAudio::Corrupted)
	{
		return corruptedFilePath();
	}
	else if(type == ViAudio::Corrected)
	{
		return correctedFilePath();
	}
	return "";
}

QString ViAudioObject::targetFilePath()
{
	QMutexLocker locker(&mMutex);
	return mTargetFile;
}

QString ViAudioObject::corruptedFilePath()
{
	QMutexLocker locker(&mMutex);
	return mCorruptedFile;
}

QString ViAudioObject::correctedFilePath()
{
	QMutexLocker locker(&mMutex);
	return mCorrectedFile;
}

void ViAudioObject::setFilePath(ViAudio::Type type, QString path)
{
	if(type == ViAudio::Target)
	{
		setTargetFilePath(path);
	}
	else if(type == ViAudio::Corrupted)
	{
		setCorruptedFilePath(path);
	}
	else if(type == ViAudio::Corrected)
	{
		setCorrectedFilePath(path);
	}
}

void ViAudioObject::setTargetFilePath(QString path)
{
	QMutexLocker locker(&mMutex);
	mTargetFile = path;
}

void ViAudioObject::setCorruptedFilePath(QString path)
{
	QMutexLocker locker(&mMutex);
	mCorruptedFile = path;
}

void ViAudioObject::setCorrectedFilePath(QString path)
{
	QMutexLocker locker(&mMutex);
	mCorrectedFile = path;
}

bool ViAudioObject::hasFile(ViAudio::Type type)
{
	return filePath(type) != "";
}

QString ViAudioObject::fileName(bool track, bool side)
{
	QString result = "";

	if(side)
	{
		result += "[Side ";
		if(mSideNumber < 10)
		{
			result += "0";
		}
		result += QString::number(mSideNumber) + "] ";
	}

	if(track)
	{
		if(mTrackNumber < 10)
		{
			result += "0";
		}
		result += QString::number(mTrackNumber) + ". ";
	}

	if(mSongInfo.artistName() == "")
	{
		result += "Unknown Artist";
	}
	else
	{
		result += mSongInfo.artistName();
	}
	result += " - ";

	if(mSongInfo.songTitle() == "")
	{
		result += "Unknown Title";
	}
	else
	{
		result += mSongInfo.songTitle();
	}

	return result;
}

QString ViAudioObject::temporaryFilePath(ViAudio::Type type)
{
	QString result = ViManager::tempPath() + id();
	if(type == ViAudio::Target)
	{
		result += "_target";
	}
	else if(type == ViAudio::Corrupted)
	{
		result += "_corrupted";
	}
	else if(type == ViAudio::Corrected)
	{
		result += "_corrected";
	}
	ViAudioCodec *codec = format(type).codec();
	if(codec == NULL)
	{
		result += ".data";
	}
	else
	{
		result += codec->extension(".");
	}
	return result;
}

/*******************************************************************************************************************

	WAVEFORM

*******************************************************************************************************************/

bool ViAudioObject::generateWaveForm(ViAudio::Type types)
{
	ViAudio::Type waveTypes = availableResources();
	QMutexLocker locker(&mMutex);
	QQueue<ViAudio::Type> instructions = decomposeTypes(waveTypes);

	mWaveInstructions.clear();
	for(int i = 0; i < instructions.size(); ++i)
	{
		if(types & instructions[i])
		{
			mWaveInstructions.append(instructions[i]);
		}
	}

	if(mWaveInstructions.size() == 0)
	{
		log("No data available to generate a wave form.");
		locker.unlock();
		emit waved();
		return false;
	}
	int decodeTypes = 0;
	for(int i = 0; i < mWaveInstructions.size(); ++i)
	{
		ViAudio::Type instruction = mWaveInstructions[i];
		locker.unlock();
		if(!hasBuffer(instruction))
		{
			decodeTypes |= instruction;
		}
		locker.relock();
	}

	if(mWaveFormer != NULL)
	{
		delete mWaveFormer;
	}
	mWaveFormer = new ViWaveFormer();
	QObject::connect(mWaveFormer, SIGNAL(finished()), this, SLOT(generateNextWaveForm()));
	QObject::connect(mWaveFormer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));

	QObject::connect(this, SIGNAL(decoded()), this, SLOT(initializeWaveForm()));
	//setProgress(1, 0.05);
	locker.unlock();
	if(decodeTypes == 0)
	{
		progress(100);
		initializeWaveForm();
		return true;
	}
	else if(decode((ViAudio::Type) decodeTypes))
	{
		return true;
	}

	progress(100);
	//setProgress(1, 0.95);
	progress(100);
	emit waved();

	return false;
}

void ViAudioObject::initializeWaveForm()
{
	QMutexLocker locker(&mMutex);
	QObject::disconnect(this, SIGNAL(decoded()), this, SLOT(initializeWaveForm()));
	locker.unlock();
	//setProgress(mWaveInstructions.size(), 0.9);
	emit statused("Generating wave form: "+QString::number(mWaveInstructions.size()));
	generateNextWaveForm();
}

void ViAudioObject::generateNextWaveForm()
{
	QMutexLocker locker(&mMutex);
	if(mWaveInstructions.isEmpty())
	{
		log("Wave forms generated.");
		if(mWaveFormer != NULL)
		{
			QObject::disconnect(mWaveFormer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
			QObject::disconnect(mWaveFormer, SIGNAL(finished()), this, SLOT(generateNextWaveForm()));
			delete mWaveFormer;
			mWaveFormer = NULL;	
		}
		locker.unlock();
		//setProgress(1, 0.05);
		progress(100);
		emit waved();
	}
	else
	{
		ViAudio::Type type = mWaveInstructions.dequeue();
		locker.unlock();
		mWaveFormer->process(thisPointer, type);
	}
}

void ViAudioObject::setWaveForm(ViAudio::Type type, ViWaveForm *form)
{
	QMutexLocker locker(&mMutex);
	if(mWaveForms.value(type, NULL) != NULL)
	{
		delete mWaveForms[type];
	}
	mWaveForms[type] = form;
}

ViWaveForm* ViAudioObject::waveForm(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mWaveForms.value(type, NULL);
}

/*******************************************************************************************************************

	CORRECTION

*******************************************************************************************************************/

void ViAudioObject::setCorrector(ViModifyProcessor *corrector)
{
	QMutexLocker locker(&mMutex);
	if(mCorrector != NULL)
	{
		delete mCorrector;
	}
	mCorrector = corrector;
	QObject::connect(mCorrector, SIGNAL(finished()), this, SLOT(endCorrect()));
	QObject::connect(mCorrector, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
    emit correctorChanged();
}

bool ViAudioObject::hasCorrector()
{
	QMutexLocker locker(&mMutex);
	return mCorrector != NULL;
}

ViModifyProcessor* ViAudioObject::corrector()
{
    return mCorrector;
}

bool ViAudioObject::correct(ViModifyProcessor *corrector)
{
	setStarted();
	if(corrector != NULL)
	{
		setCorrector(corrector);
	}

	if(!hasCorrector())
	{
		log("No corrector was specified.", QtWarningMsg);
		emit corrected();
		setFinished();
		return false;
	}
	else if(!hasBuffer(ViAudio::Corrupted))
	{
		log("No corrupted buffer is available for correction.", QtWarningMsg);
		emit corrected();
		setFinished();
		return false;
	}

	logStatus("Correcting track.");
	correctedBuffer()->setFormat(corruptedFormat());
	mCorrector->process(thisPointer, ViAudio::Corrupted, ViAudio::Corrected);
	return true;
}

void ViAudioObject::endCorrect()
{
	log("Track corrected.");
	emit corrected();
	setFinished();
}

/*******************************************************************************************************************

    CORRELATE

*******************************************************************************************************************/

ViCorrelation ViAudioObject::correlation(QString correlator, ViAudio::Type type1, ViAudio::Type type2)
{
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        if(mCorrelations[i].type1() == type1 && mCorrelations[i].type2() == type2)
        {
            return mCorrelations[i].correlation(correlator);
        }
    }
    return ViCorrelation();
}

ViCorrelationGroup ViAudioObject::correlation(ViAudio::Type type1, ViAudio::Type type2)
{
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        if(mCorrelations[i].type1() == type1 && mCorrelations[i].type2() == type2)
        {
            return mCorrelations[i];
        }
    }
    return ViCorrelationGroup();
}

ViCorrelationGroups ViAudioObject::ViAudioObject::correlations()
{
    return mCorrelations;
}

void ViAudioObject::clearCorrelators()
{
    viDeleteAll(mCorrelators);
}

void ViAudioObject::addCorrelator(ViCorrelator *correlator)
{
    mCorrelators.append(correlator);
    QObject::connect(correlator, SIGNAL(finished()), this, SLOT(correlateNext()));
    QObject::connect(correlator, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

bool ViAudioObject::hasCorrelator()
{
    return !mCorrelators.isEmpty();
}

int ViAudioObject::correlatorCount()
{
    return mCorrelators.size();
}

bool ViAudioObject::correlate(ViCorrelator *correlator)
{
    clearCorrelators();
    addCorrelator(correlator);
    return correlate();
}

bool ViAudioObject::correlate(QList<ViCorrelator*> correlators)
{
    clearCorrelators();
    for(int i = 0; i < correlators.size(); ++i)
    {
        addCorrelator(correlators[i]);
    }
    return correlate();
}

bool ViAudioObject::correlate()
{
	setStarted();
    if(!hasCorrelator())
    {
        log("No correlators available for correlation", QtCriticalMsg);
        emit correlated();
		setFinished();
        return false;
    }

    mCorrelations.clear();
    mCorrelationTypes.clear();
    if(hasBuffer(ViAudio::Target) && hasBuffer(ViAudio::Corrected))
    {
        mCorrelationTypes.enqueue(QPair<ViAudio::Type, ViAudio::Type>(ViAudio::Target, ViAudio::Corrected));
    }
    if(hasBuffer(ViAudio::Target) && hasBuffer(ViAudio::Corrupted))
    {
        mCorrelationTypes.enqueue(QPair<ViAudio::Type, ViAudio::Type>(ViAudio::Target, ViAudio::Corrupted));
    }

    if(mCorrelationTypes.isEmpty())
    {
        log("No buffers available for correlation", QtCriticalMsg);
        emit correlated();
		setFinished();
        return false;
    }

    mCurrentCorrelator = 0;
    mCurrentCorrelation = 0;

    setProgress(mCorrelationTypes.size() * correlatorCount());
    logStatus("Correlating track");
    correlateNext();
    return true;
}

void ViAudioObject::correlateNext()
{
    if(mCurrentCorrelator > 0 || mCurrentCorrelation > 0)
    {
        int correlator = mCurrentCorrelator - 1;
        if(correlator < 0) correlator = correlatorCount() - 1;
        ViCorrelation correlation = mCorrelators[correlator]->correlation();
        mCorrelations.last().add(mCorrelators[correlator], correlation);
        emit changed();
    }

    if(mCurrentCorrelation >= mCorrelationTypes.size())
    {
        log("The track was correlated.");
        emit correlated();
		setFinished();
        return;
    }

    QPair<ViAudio::Type, ViAudio::Type> types = mCorrelationTypes[mCurrentCorrelation];
    mCorrelators[mCurrentCorrelator]->process(thisPointer, types.first, types.second);

    if(mCurrentCorrelator == 0)
    {
        mCorrelations.append(ViCorrelationGroup(types));
    }

    ++mCurrentCorrelator;
    if(mCurrentCorrelator >= correlatorCount())
    {
        mCurrentCorrelator = 0;
        ++mCurrentCorrelation;
    }
}

/*******************************************************************************************************************

	SONG INFO

*******************************************************************************************************************/

ViSongInfo& ViAudioObject::songInfo()
{
	QMutexLocker locker(&mMutex);
	return mSongInfo;
}

void ViAudioObject::setSongInfo(ViSongInfo info)
{
	QMutexLocker locker(&mMutex);
	mSongInfo = info;
}

void ViAudioObject::detectSongInfo()
{
	QMutexLocker locker(&mMutex);
	mIsDetectingInfo = true;
	if(mMetadataer != NULL)
	{
		delete mMetadataer;
	}
	mMetadataer = new ViMetadataer();
	QObject::connect(mMetadataer, SIGNAL(finished(bool)), this, SLOT(finishDetection(bool)));

	locker.unlock();
	QQueue<ViBuffer*> buffers;
	if(hasBuffer(ViAudio::Target))
	{
		buffers.enqueue(buffer(ViAudio::Target));
	}
	if(hasBuffer(ViAudio::Corrected))
	{
		buffers.enqueue(buffer(ViAudio::Corrected));
	}
	if(hasBuffer(ViAudio::Corrupted))
	{
		buffers.enqueue(buffer(ViAudio::Corrupted));
	}
	locker.relock();
	mMetadataer->detect(buffers);
}

void ViAudioObject::finishDetection(bool success)
{
	QMutexLocker locker(&mMutex);
	if(mMetadataer != NULL)
	{
		if(success)
		{
			mMetadata = mMetadataer->metadata();
			mSongInfo.setSongTitle(mMetadata.title());
			mSongInfo.setArtistName(mMetadata.artist());
			mSongInfo.setImagePath(mMetadata.cover());
		}
		delete mMetadataer;
		mMetadataer = NULL;
		mIsDetectingInfo = false;
		locker.unlock();
		emit infoed(success);
	}
}

bool ViAudioObject::isDetectingSongInfo()
{
	QMutexLocker locker(&mMutex);
	return mIsDetectingInfo;
}


bool ViAudioObject::hasSongInfo()
{
    QMutexLocker locker(&mMutex);
    return mSongInfo.isValid();
}

void ViAudioObject::setSideNumber(int side)
{
	QMutexLocker locker(&mMutex);
	mSideNumber = side;
}

int ViAudioObject::sideNumber()
{
	QMutexLocker locker(&mMutex);
	return mSideNumber;
}

void ViAudioObject::setTrackNumber(int track)
{
	QMutexLocker locker(&mMutex);
	mTrackNumber = track;
}

int ViAudioObject::trackNumber()
{
	QMutexLocker locker(&mMutex);
	return mTrackNumber;
}












void ViAudioObject::setSong(bool song)
{
	QMutexLocker locker(&mMutex);
	mIsSong = song;
}

bool ViAudioObject::isSong()
{
	QMutexLocker locker(&mMutex);
	return mIsSong;
}

bool ViAudioObject::isUsed(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	return	(mTargetBuffer != NULL && mTargetBuffer->streamCount(mode) > 0) ||
			(mCorruptedBuffer != NULL && mCorruptedBuffer->streamCount(mode) > 0) ||
			(mCorrectedBuffer != NULL && mCorrectedBuffer->streamCount(mode) > 0) ||
			(mTemporaryBuffer != NULL && mTemporaryBuffer->streamCount(mode) > 0);
}
