#include <viaudioobject.h>
#include <viwaveformer.h>
#include <vimetadataer.h>
#include <vifouriercrossaligner.h>
#include <viacoustididentifier.h>
#include <vienmfpidentifier.h>
#include <vimanager.h>
#include <viaudiocodec.h>
#include <QSet>

/*******************************************************************************************************************

	CONSTRUCTORS, DESTRUCTORS & GENERAL

*******************************************************************************************************************/

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject(), ViId()
{
	setAutoDestruct(autoDestruct);
	mIsFinished = false;
	mIsSong = false;

	mInputType = ViAudioObject::Undefined;
	mOutputType = ViAudioObject::Undefined;

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

	mSideNumber = 0;
	mTrackNumber = 0;

	setAligner(new ViFourierCrossAligner());

	QObject::connect(this, SIGNAL(encoded()), this, SIGNAL(finished()));
	QObject::connect(this, SIGNAL(decoded()), this, SIGNAL(finished()));
	QObject::connect(this, SIGNAL(aligned()), this, SIGNAL(finished()));
	QObject::connect(this, SIGNAL(corrected()), this, SIGNAL(finished()));
}

ViAudioObject::~ViAudioObject()
{
	clearBuffers(mDestructType);

	qDeleteAll(mWaveForms);
	mWaveForms.clear();

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
}

ViAudioObjectPointer ViAudioObject::create(ViAudioObject *object)
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(object);
	pointer.setUnusedLimit(2);
	pointer->thisPointer = pointer;
	return pointer;
}

ViAudioObjectPointer ViAudioObject::create(bool autoDestruct)
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(new ViAudioObject(autoDestruct));
	pointer.setUnusedLimit(2);
	pointer->thisPointer = pointer;
	return pointer;
}

ViAudioObjectPointer ViAudioObject::createNull()
{
	ViAudioObjectPointer pointer = ViAudioObjectPointer(NULL);
	pointer.setUnusedLimit(2);
	pointer->thisPointer = pointer;
	return pointer;
}

QQueue<ViAudioObject::Type> ViAudioObject::decomposeTypes(ViAudioObject::Type type, ViAudioObject::Type exclude)
{
	QQueue<ViAudioObject::Type> result;
	if(type & ViAudioObject::Target && !(exclude & ViAudioObject::Target))
	{
		result.enqueue(ViAudioObject::Target);
	}
	if(type & ViAudioObject::Corrupted && !(exclude & ViAudioObject::Corrupted))
	{
		result.enqueue(ViAudioObject::Corrupted);
	}
	if(type & ViAudioObject::Corrected && !(exclude & ViAudioObject::Corrected))
	{
		result.enqueue(ViAudioObject::Corrected);
	}
	if(type & ViAudioObject::Temporary && !(exclude & ViAudioObject::Temporary))
	{
		result.enqueue(ViAudioObject::Temporary);
	}
	return result;
}

ViAudioObject::Type ViAudioObject::composeTypes(QQueue<ViAudioObject::Type> &types, ViAudioObject::Type exclude)
{
	int result = 0;
	for(int i = 0; i < types.size(); ++i)
	{
		if(!(types[i] & exclude))
		{
			result |= types[i];
		}
	}
	return (ViAudioObject::Type) result;
}

bool ViAudioObject::hasResource(ViAudioObject::Type type)
{
	return (filePath(type) != "") || (buffer(type, true) != NULL);
}

ViAudioObject::Resource ViAudioObject::resourceAvailable(ViAudioObject::Type type)
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

ViAudioObject::Type ViAudioObject::availableResources(ViAudioObject::Resource resource)
{
	int result = 0;
	if(resourceAvailable(ViAudioObject::Target) & resource)
	{
		result |= ViAudioObject::Target;
	}
	if(resourceAvailable(ViAudioObject::Corrupted) & resource)
	{
		result |= ViAudioObject::Corrupted;
	}
	if(resourceAvailable(ViAudioObject::Corrected) & resource)
	{
		result |= ViAudioObject::Corrected;
	}
	if(result == 0)
	{
		return ViAudioObject::Undefined;
	}
	return (ViAudioObject::Type) result;
}

qreal ViAudioObject::length(ViAudioPosition::Unit unit)
{
	qreal maximum = 0;
	if(hasBuffer(ViAudioObject::Target) && buffer(ViAudioObject::Target)->size() > maximum)
	{
		maximum = length(ViAudioObject::Target, unit);
	}
	if(hasBuffer(ViAudioObject::Corrupted) && buffer(ViAudioObject::Corrupted)->size() > maximum)
	{
		maximum = length(ViAudioObject::Corrupted, unit);
	}
	if(hasBuffer(ViAudioObject::Corrected) && buffer(ViAudioObject::Corrected)->size() > maximum)
	{
		maximum = length(ViAudioObject::Corrected, unit);
	}
	return maximum;
}

qreal ViAudioObject::length(ViAudioObject::Type type, ViAudioPosition::Unit unit)
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
		mDestructType = ViAudioObject::All;
	}
	else
	{
		mDestructType = ViAudioObject::Undefined;
	}
}

void ViAudioObject::addDestructRule(ViAudioObject::Type type, bool destruct)
{
	QMutexLocker locker(&mMutex);
	QSet<ViAudioObject::Type> set;
	QSet<ViAudioObject::Type> values;
	values.insert(ViAudioObject::Target);
	values.insert(ViAudioObject::Corrupted);
	values.insert(ViAudioObject::Corrected);
	values.insert(ViAudioObject::Temporary);

	//Old Values
	foreach(const ViAudioObject::Type &value, values)
	{
		if(mDestructType & value)
		{
			set.insert(value);
		}
	}

	//New Values
	foreach(const ViAudioObject::Type &value, values)
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
		mDestructType = ViAudioObject::Undefined;
	}
	else
	{
		QList<ViAudioObject::Type> list = set.toList();
		int temp = list[0];
		for(int i = 1; i < list.size(); ++i)
		{
			temp |= list[i];
		}
		mDestructType = (ViAudioObject::Type) temp;
	}
}

/*******************************************************************************************************************

	FORMATS

*******************************************************************************************************************/

void ViAudioObject::setOutputFormat(ViAudioFormat format)
{
	mOutputFormat = format;
}

ViAudioFormat ViAudioObject::format(ViAudioObject::Type type)
{
	if(type == ViAudioObject::Target)
	{
		return targetFormat();
	}
	else if(type == ViAudioObject::Corrupted)
	{
		return corruptedFormat();
	}
	else if(type == ViAudioObject::Corrected)
	{
		return correctedFormat();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::targetFormat()
{
	ViBuffer *buffer = targetBuffer(true);
	if(buffer != NULL)
	{
		return buffer->format();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::corruptedFormat()
{
	ViBuffer *buffer = corruptedBuffer(true);
	if(buffer != NULL)
	{
		return buffer->format();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::correctedFormat()
{
	ViBuffer *buffer = correctedBuffer(true);
	if(buffer != NULL)
	{
		return buffer->format();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::inputFormat()
{
	ViBuffer *buffer = inputBuffer(true);
	if(buffer != NULL)
	{
		return buffer->format();
	}
	return ViAudioFormat();
}

ViAudioFormat ViAudioObject::outputFormat()
{
	ViBuffer *buffer = outputBuffer(true);
	if(buffer != NULL)
	{
		return buffer->format();
	}
	return mOutputFormat;
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

bool ViAudioObject::encode(ViAudioFormat format, bool clearWhenFinished)
{
	setOutputFormat(format);
	return encode(ViAudioObject::Undefined, clearWhenFinished);
}

bool ViAudioObject::encode(ViAudioObject::Type type, ViAudioFormat format, bool clearWhenFinished)
{
	setOutputFormat(format);
	return encode(type, clearWhenFinished);
}

bool ViAudioObject::encode(ViAudioObject::Type type, bool clearWhenFinished)
{
	QMutexLocker locker(&mMutex);

	mClearEncodedBuffer = clearWhenFinished;
	mPreviousEncodedType = ViAudioObject::Undefined;

	if(type == ViAudioObject::Undefined)
	{
		locker.unlock();
		type = outputType();
		locker.relock();
	}
	if(type == ViAudioObject::Undefined)
	{
		locker.unlock();
		type = inputType();
		locker.relock();
	}
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
		emit progressed(100);
		emit encoded();
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
		mPreviousEncodedType = ViAudioObject::Undefined;
	}

	if(mCodingInstructions.isEmpty())
	{
		log("Buffers encoded.");
		delete mEncoder;
		mEncoder = NULL;
		locker.unlock();
		emit encoded();
	}
	else
	{
		ViAudioObject::Type type = mCodingInstructions.dequeue();
		locker.unlock();
		
		QString thePath = filePath(type);
		ViBuffer *theBuffer = buffer(type, true);
		locker.relock();
		if(mOutputFormat.isValid(true))
		{
			mEncoder->encode(theBuffer, thePath, mOutputFormat, 0, mSongInfo);
		}
		else
		{
			mEncoder->encode(theBuffer, thePath, theBuffer->format(), 0, mSongInfo);
		}
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

bool ViAudioObject::decode(ViAudioObject::Type type)
{
	if(type == ViAudioObject::Undefined)
	{
		type = inputType();
	}
	QMutexLocker locker(&mMutex);
	mCodingInstructions = decomposeTypes(type);

	for(int i = 0; i < mCodingInstructions.size(); ++i)
	{
		ViAudioObject::Type type = mCodingInstructions[i];
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
		emit progressed(100);
		emit decoded();
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
		emit progress(100);
		emit decoded();
	}
	else
	{
		ViAudioObject::Type type = mCodingInstructions.dequeue();
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
	if(aligner != NULL)
	{
		setAligner(aligner);
	}
	if(!hasAligner())
	{
		log("No aligner was specified.");
		emit aligned();
		emit progressed(100);
		return false;
	}

	mAlignerTypes = availableResources(ViAudioObject::Buffer);
	mAlignerInstructions = decomposeTypes(mAlignerTypes);
	if(mAlignerInstructions.size() < 2)
	{
		log("At least two buffers are needed for alignment.");
		emit aligned();
		emit progressed(100);
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
	}
	else
	{
		mAligner->align(buffer(mMainAligner), buffer(mAlignerInstructions.dequeue()));
	}
}

/*******************************************************************************************************************

	INPUT & OUTPUT

*******************************************************************************************************************/

void ViAudioObject::setType(ViAudioObject::Type input, ViAudioObject::Type output)
{
	setInputType(input);
	setOutputType(output);
}

void ViAudioObject::setInputType(ViAudioObject::Type type)
{
	QMutexLocker locker(&mMutex);
	mInputType = type;
}

void ViAudioObject::setOutputType(ViAudioObject::Type type)
{
	QMutexLocker locker(&mMutex);
	mOutputType = type;
}

ViAudioObject::Type ViAudioObject::inputType()
{
	QMutexLocker locker(&mMutex);
	return mInputType;
}

ViAudioObject::Type ViAudioObject::outputType()
{
	QMutexLocker locker(&mMutex);
	return mOutputType;
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
	mProgressParts = parts;
}

/*******************************************************************************************************************

	BUFFERS

*******************************************************************************************************************/

void ViAudioObject::transferBuffer(ViAudioObjectPointer object, ViAudioObject::Type type)
{
	QMutexLocker locker(&mMutex);
	if(type == ViAudioObject::Undefined)
	{
		locker.unlock();
		type = object->outputType();
		locker.relock();
	}
	object->addDestructRule(type, false);
	locker.unlock();
	setBuffer(type, object->buffer(type));
	setFilePath(type, object->filePath(type));
}

ViBuffer* ViAudioObject::buffer(ViAudioObject::Type type, bool dontCreate)
{
	if(type == ViAudioObject::Target)
	{
		return targetBuffer(dontCreate);
	}
	else if(type == ViAudioObject::Corrupted)
	{
		return corruptedBuffer(dontCreate);
	}
	else if(type == ViAudioObject::Corrected)
	{
		return correctedBuffer(dontCreate);
	}
	else if(type == ViAudioObject::Temporary)
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
	}
	return mTargetBuffer;
}

ViBuffer* ViAudioObject::corruptedBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mCorruptedBuffer == NULL)
	{
		mCorruptedBuffer = new ViBuffer();
	}
	return mCorruptedBuffer;
}

ViBuffer* ViAudioObject::correctedBuffer(bool dontCreate)
{
	QMutexLocker locker(&mMutex);
	if(!dontCreate && mCorrectedBuffer == NULL)
	{
		mCorrectedBuffer = new ViBuffer();
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

ViBuffer* ViAudioObject::inputBuffer(bool dontCreate)
{
	if(mInputType == ViAudioObject::Target)
	{
		return targetBuffer(dontCreate);
	}
	else if(mInputType == ViAudioObject::Corrupted)
	{
		return corruptedBuffer(dontCreate);
	}
	else if(mInputType == ViAudioObject::Corrected)
	{
		return correctedBuffer(dontCreate);
	}
	else if(mInputType == ViAudioObject::Temporary)
	{
		return temporaryBuffer(dontCreate);
	}
	return NULL;
}

ViBuffer* ViAudioObject::outputBuffer(bool dontCreate)
{
	if(mOutputType == ViAudioObject::Target)
	{
		return targetBuffer(dontCreate);
	}
	else if(mOutputType == ViAudioObject::Corrupted)
	{
		return corruptedBuffer(dontCreate);
	}
	else if(mOutputType == ViAudioObject::Corrected)
	{
		return correctedBuffer(dontCreate);
	}
	else if(mOutputType == ViAudioObject::Temporary)
	{
		return temporaryBuffer(dontCreate);
	}
	return NULL;
}

void ViAudioObject::setBuffer(ViAudioObject::Type type, ViBuffer *buffer)
{
	if(type == ViAudioObject::Target)
	{
		setTargetBuffer(buffer);
	}
	else if(type == ViAudioObject::Corrupted)
	{
		setCorruptedBuffer(buffer);
	}
	else if(type == ViAudioObject::Corrected)
	{
		setCorrectedBuffer(buffer);
	}
}

void ViAudioObject::setTargetBuffer(ViBuffer *buffer)
{
	clearTargetBuffer();
	QMutexLocker locker(&mMutex);
	mTargetBuffer = buffer;
}

void ViAudioObject::setCorruptedBuffer(ViBuffer *buffer)
{
	clearCorruptedBuffer();
	QMutexLocker locker(&mMutex);
	mCorruptedBuffer = buffer;
}

void ViAudioObject::setCorrectedBuffer(ViBuffer *buffer)
{
	clearCorrectedBuffer();
	QMutexLocker locker(&mMutex);
	mCorrectedBuffer = buffer;
}

void ViAudioObject::clearBuffers(ViAudioObject::Type type)
{
	if(type & ViAudioObject::Target)
	{
		clearTargetBuffer();
	}
	if(type & ViAudioObject::Corrupted)
	{
		clearCorruptedBuffer();
	}
	if(type & ViAudioObject::Corrected)
	{
		clearCorrectedBuffer();
	}
	if(type & ViAudioObject::Temporary)
	{
		clearTemporaryBuffer();
	}
}

void ViAudioObject::clearBuffer(ViAudioObject::Type type)
{
	if(type == ViAudioObject::Target)
	{
		clearTargetBuffer();
	}
	else if(type == ViAudioObject::Corrupted)
	{
		clearCorruptedBuffer();
	}
	else if(type == ViAudioObject::Corrected)
	{
		clearCorrectedBuffer();
	}
	else if(type == ViAudioObject::Temporary)
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

bool ViAudioObject::hasBuffer(ViAudioObject::Type type)
{
	return buffer(type, true) != NULL;
}

bool ViAudioObject::hasInputBuffer()
{
	return hasBuffer(mInputType);
}

bool ViAudioObject::hasOutputBuffer()
{
	return hasBuffer(mOutputType);
}

/*******************************************************************************************************************

	FILES

*******************************************************************************************************************/

QString ViAudioObject::filePath(ViAudioObject::Type type)
{
	if(type == ViAudioObject::Target)
	{
		return targetFilePath();
	}
	else if(type == ViAudioObject::Corrupted)
	{
		return corruptedFilePath();
	}
	else if(type == ViAudioObject::Corrected)
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

void ViAudioObject::setFilePath(ViAudioObject::Type type, QString path)
{
	if(type == ViAudioObject::Target)
	{
		setTargetFilePath(path);
	}
	else if(type == ViAudioObject::Corrupted)
	{
		setCorruptedFilePath(path);
	}
	else if(type == ViAudioObject::Corrected)
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

bool ViAudioObject::hasFile(ViAudioObject::Type type)
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

QString ViAudioObject::temporaryFilePath(ViAudioObject::Type type)
{
	QString result = ViManager::tempPath() + id();
	if(type == ViAudioObject::Target)
	{
		result += "_target";
	}
	else if(type == ViAudioObject::Corrupted)
	{
		result += "_corrupted";
	}
	else if(type == ViAudioObject::Corrected)
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

bool ViAudioObject::generateWaveForm(ViAudioObject::Type types)
{
	ViAudioObject::Type waveTypes = availableResources();
	QMutexLocker locker(&mMutex);
	QQueue<ViAudioObject::Type> instructions = decomposeTypes(waveTypes);

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
		ViAudioObject::Type instruction = mWaveInstructions[i];
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
	else if(decode((ViAudioObject::Type) decodeTypes))
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
		ViAudioObject::Type type = mWaveInstructions.dequeue();
		locker.unlock();
		mWaveFormer->process(thisPointer, type);
	}
}

void ViAudioObject::setWaveForm(ViAudioObject::Type type, ViWaveForm *form)
{
	QMutexLocker locker(&mMutex);
	if(mWaveForms.value(type, NULL) != NULL)
	{
		delete mWaveForms[type];
	}
	mWaveForms[type] = form;
}

ViWaveForm* ViAudioObject::waveForm(ViAudioObject::Type type)
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
}

bool ViAudioObject::hasCorrector()
{
	QMutexLocker locker(&mMutex);
	return mCorrector != NULL;
}

bool ViAudioObject::correct(ViModifyProcessor *corrector)
{
	if(corrector != NULL)
	{
		setCorrector(corrector);
	}

	if(!hasCorrector())
	{
		log("No corrector was specified.", QtWarningMsg);
		progress(100);
		emit corrected();
		return false;
	}
	else if(!hasBuffer(ViAudioObject::Corrupted))
	{
		log("No corrupted buffer is available for correction.", QtWarningMsg);
		progress(100);
		emit corrected();
		return false;
	}

	logStatus("Correcting track.");
	correctedBuffer()->setFormat(corruptedFormat());
	mCorrector->process(thisPointer, ViAudioObject::Corrupted, ViAudioObject::Corrected);
	return true;
}

void ViAudioObject::endCorrect()
{
	log("Track corrected.");
	progress(100);
	emit corrected();
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
	if(hasBuffer(ViAudioObject::Target))
	{
		buffers.enqueue(buffer(ViAudioObject::Target));
	}
	if(hasBuffer(ViAudioObject::Corrected))
	{
		buffers.enqueue(buffer(ViAudioObject::Corrected));
	}
	if(hasBuffer(ViAudioObject::Corrupted))
	{
		buffers.enqueue(buffer(ViAudioObject::Corrupted));
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

bool ViAudioObject::isFinished()
{
	QMutexLocker locker(&mMutex);
	return mIsFinished;
}

bool ViAudioObject::isUsed(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	return	(mTargetBuffer != NULL && mTargetBuffer->streamCount(mode) > 0) ||
			(mCorruptedBuffer != NULL && mCorruptedBuffer->streamCount(mode) > 0) ||
			(mCorrectedBuffer != NULL && mCorrectedBuffer->streamCount(mode) > 0) ||
			(mTemporaryBuffer != NULL && mTemporaryBuffer->streamCount(mode) > 0);
}

void ViAudioObject::addCorrelation(const ViElement &correlation)
{
	QMutexLocker locker(&mMutex);
	mCorrelations.append(correlation);
}

ViElementList& ViAudioObject::correlations()
{
	QMutexLocker locker(&mMutex);
	return mCorrelations;
}
