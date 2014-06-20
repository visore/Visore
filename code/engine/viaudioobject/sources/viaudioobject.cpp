#include <viaudioobject.h>
#include <viwaveformer.h>
#include <vispectrumanalyzer.h>
#include <vimetadataer.h>
#include <vifouriercrossaligner.h>
#include <viacoustididentifier.h>
#include <vienmfpidentifier.h>
#include <vimanager.h>
#include <viaudiocodec.h>
#include <vicorrelator.h>
#include <vinoisedetector.h>
#include <QSet>

/*******************************************************************************************************************

	CONSTRUCTORS, DESTRUCTORS & GENERAL

*******************************************************************************************************************/

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject(), ViId()
{
	setAutoDestruct(autoDestruct);
	mIsSong = false;

	QList<ViAudio::Type> types = ViAudio::types();
	for(int i = 0; i < types.size(); ++i)
	{
		mBuffers[types[i]] = NULL;
		mFiles[types[i]] = "";
	}

	mEncoder = NULL;
	mDecoder = NULL;
	mAligner = NULL;
	mWaveFormer = NULL;
	mSpectrumAnalyzer = NULL;
	mMetadataer = NULL;
	mCorrector = NULL;

	mProgressParts = 1;
	mProgress = 0;
	mIsBusy = false;

	mSideNumber = 0;
	mTrackNumber = 0;

	mNoiseDetector = NULL;

	mCustomMaskCreator = new ViCustomMaskCreator();
	QObject::connect(mCustomMaskCreator, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
	QObject::connect(mCustomMaskCreator, SIGNAL(finished()), this, SLOT(setFinished()));
	QObject::connect(mCustomMaskCreator, SIGNAL(finished()), this, SIGNAL(customGenerated()));

	setAligner(new ViFourierCrossAligner());
}

ViAudioObject::~ViAudioObject()
{
	clearBuffer(mDestructType);
	clearWaves();
	clearSpectrums();
	clearCorrelators();

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

	if(mSpectrumAnalyzer != NULL)
	{
		delete mSpectrumAnalyzer;
		mSpectrumAnalyzer = NULL;
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

	if(mNoiseDetector != NULL)
	{
		delete mNoiseDetector;
		mNoiseDetector = NULL;
	}

	if(mCustomMaskCreator != NULL)
	{
		delete mCustomMaskCreator;
		mCustomMaskCreator = NULL;
	}
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
    pointer.setUnusedLimit(1);
	return pointer;
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
	QList<ViAudio::Type> types = ViAudio::types();
	for(int i = 0; i < types.size(); ++i)
	{
		if(resourceAvailable(types[i]) & resource)
		{
			result |= types[i];
		}
	}
	if(result == 0) return ViAudio::Undefined;
	return (ViAudio::Type) result;
}

qreal ViAudioObject::length(ViAudioPosition::Unit unit)
{
	QMutexLocker locker(&mMutex);
	qreal current, maximum = 0;
	ViAudio::Type type;
	foreach(type, mBuffers.keys())
	{
		locker.unlock();
		current = length(type, unit);
		locker.relock();
		if(current > maximum) maximum = current;
	}
	return maximum;
}

qreal ViAudioObject::length(ViAudio::Type type, ViAudioPosition::Unit unit)
{
	QMutexLocker locker(&mMutex);
	if(mBuffers[type] != NULL)
	{
		return ViAudioPosition::convertPosition(mBuffers[type]->size(), ViAudioPosition::Bytes, unit, mBuffers[type]->format());
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
	values.insert(ViAudio::Noise);
	values.insert(ViAudio::NoiseMask);

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

ViAudioFormat ViAudioObject::format(ViAudio::Type type)
{
	if(type == ViAudio::All)
	{
		QList<ViAudio::Type> types = ViAudio::types(availableResources(ViAudioObject::Buffer));
		QMutexLocker locker(&mMutex);
		for(int i = 0; i < types.size(); ++i)
		{
			if(mBuffers[types[i]]->format().isValid()) return mBuffers[types[i]]->format();
		}
	}
	else if(hasBuffer(type))
	{
		QMutexLocker locker(&mMutex);
		return mBuffers[type]->format();
	}
	return ViAudioFormat();
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
	mCodingInstructions = ViAudio::types(type);
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
		else
		{
			setFilePath(mCodingInstructions[i], temporaryFilePath(mCodingInstructions[i]));
		}
		locker.relock();
	}
	if(mCodingInstructions.isEmpty())
	{
		mCodingInstructions.clear();
		locker.unlock();
		setFinished();
		emit encoded();
		return false;
	}

	setEncoder(new ViAudioCoder());
	
	setProgress(mCodingInstructions.size());
	locker.unlock();
	log("Starting encoding.");
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
		emit changed();
		setFinished();
		emit encoded();
	}
	else
	{
		mPreviousEncodedType = mCodingInstructions.takeFirst();
		locker.unlock();
		QString thePath = filePath(mPreviousEncodedType);
		ViBuffer *theBuffer = buffer(mPreviousEncodedType, true);
		locker.relock();
		log("Encoding track: " + thePath);
		mEncoder->encode(theBuffer, thePath, theBuffer->format(), mMetadata);
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
	mCodingInstructions = ViAudio::types(type);

	for(int i = 0; i < mCodingInstructions.size(); ++i)
	{
		ViAudio::Type type = mCodingInstructions[i];
		locker.unlock();
		if((hasBuffer(type) && !buffer(type)->isEmpty()) || !hasFile(type))
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
		setFinished();
		emit decoded();
		return false;
	}

	setDecoder(new ViAudioCoder());
	setProgress(mCodingInstructions.size());
	locker.unlock();
	log("Starting decoding.");
	decodeNext();
	return true;
}

void ViAudioObject::decodeNext()
{
	QMutexLocker locker(&mMutex);
	if(mCodingInstructions.isEmpty())
	{
		log("Tracks decoded.");
		delete mDecoder;
		mDecoder = NULL;
		locker.unlock();
		emit decoded();
		setFinished();
	}
	else
	{
		ViAudio::Type type = mCodingInstructions.takeFirst();
		locker.unlock();
		QString thePath = filePath(type);
		clearBuffer(type);
		ViBuffer *theBuffer = buffer(type);
		locker.relock();
		log("Decoding track: " + thePath);
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
		setFinished();
		emit aligned();
		return false;
	}

	mAlignerInstructions.clear();
	if(hasBuffer(ViAudio::Target)) mAlignerInstructions.append(ViAudio::Target);
	if(hasBuffer(ViAudio::Corrected)) mAlignerInstructions.append(ViAudio::Corrected);
	if(hasBuffer(ViAudio::Corrupted)) mAlignerInstructions.append(ViAudio::Corrupted);
	if(mAlignerInstructions.size() < 2)
	{
		log("At least two buffers are needed for alignment.");
		setFinished();
		emit aligned();
		return false;
	}

	logStatus("Aligning track.");
	mMainAligner = mAlignerInstructions.takeFirst();
	alignNext();
	return false;
}

void ViAudioObject::alignNext()
{
	if(mAlignerInstructions.isEmpty())
	{
		log("Tracks aligned.");
		setFinished();
		emit aligned();
	}
	else
	{
		mAligner->align(buffer(mMainAligner), buffer(mAlignerInstructions.takeFirst()));
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
	//emit statused(message);
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
	QMutexLocker locker(&mMutex);
	if(!dontCreate)
	{
		if(mBuffers[type] == NULL)
		{
			locker.unlock();
			ViAudioFormat newFormat = format();
			locker.relock();
			mBuffers[type] = new ViBuffer();
			if(newFormat.isValid()) mBuffers[type]->setFormat(newFormat);
		}
	}
	return mBuffers[type];
}

void ViAudioObject::setBuffer(ViAudio::Type type, ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mBuffers[type] = buffer;
}

void ViAudioObject::clearBuffers(ViAudio::Type type)
{
	QList<ViAudio::Type> types = ViAudio::types(type);
	for(int i = 0; i < types.size(); ++i)
	{
		clearBuffer(types[i]);
	}
	setFinished();
}

void ViAudioObject::clearBuffers(int type)
{
	clearBuffers((ViAudio::Type) type);
}

void ViAudioObject::clearBuffer(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	if(mBuffers[type] != NULL)
	{
		delete mBuffers[type];
		mBuffers[type] = NULL;
	}
}

bool ViAudioObject::hasBuffer(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mBuffers[type] != NULL;
}

qint64 ViAudioObject::bufferSize(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	if(mBuffers[type] != NULL)
	{
		return mBuffers[type]->size();
	}
	return 0;
}

/*******************************************************************************************************************

	FILES

*******************************************************************************************************************/

QString ViAudioObject::filePath(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mFiles[type];
}

void ViAudioObject::setFilePath(ViAudio::Type type, QString path)
{
	QMutexLocker locker(&mMutex);
	mFiles[type] = path;
}

bool ViAudioObject::hasFile(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mFiles[type] != "";
}

qint64 ViAudioObject::fileSize(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	QFile file(mFiles[type]);
	return file.size();
}

QString ViAudioObject::fileName(bool track, bool side)
{
	QMutexLocker locker(&mMutex);
	QString result = "";

	if(side)
	{
		result += "[Side ";
		if(mSideNumber < 10) result += "0";
		result += QString::number(mSideNumber) + "] ";
	}

	if(track)
	{
		if(mTrackNumber < 10) result += "0";
		result += QString::number(mTrackNumber) + ". ";
	}

	result += mMetadata.artist();
	result += " - ";
	result += mMetadata.title();

	return result;
}

QString ViAudioObject::temporaryFilePath(ViAudio::Type type)
{
	QString result = ViManager::tempDataPath() + id() + "_" + ViAudio::toString(type).toLower();
	ViAudioCodec *codec = format(type).codec();
	if(codec == NULL) result += ".data";
	else result += codec->extension(".");
	return result;
}

/*******************************************************************************************************************

	WAVEFORM

*******************************************************************************************************************/

bool ViAudioObject::generateWave(ViAudio::Type types, const bool &force)
{
	setStarted();
	QMutexLocker locker(&mMutex);

	mWaveInstructions.clear();
	QList<ViAudio::Type> instructions;
	QList<ViAudio::Type> decomposeInstructions = ViAudio::types(types);

	for(int i = 0; i < decomposeInstructions.size(); ++i)
	{
		locker.unlock();
		bool hasTheBuffer = hasBuffer(decomposeInstructions[i]);
		locker.relock();
		if(hasTheBuffer) instructions.append(decomposeInstructions[i]);
	}

	if(instructions.size() == 0)
	{
		log("No data available to generate the wave.");
		locker.unlock();
		setFinished();
		emit waved();
		return false;
	}

	if(!force)
	{
		for(int i = 0; i < instructions.size(); ++i)
		{
			locker.unlock();
			bool hasTheWave = hasWave(instructions[i]);
			locker.relock();
			if(!hasTheWave) mWaveInstructions.append(instructions[i]);
		}
		if(mWaveInstructions.size() == 0)
		{
			locker.unlock();
			setFinished();
			emit waved();
			return false;
		}
	}
	else
	{
		mWaveInstructions = instructions;
	}
	setProgress(mWaveInstructions.size());

	if(mWaveFormer != NULL)
	{
		delete mWaveFormer;
	}
	mWaveFormer = new ViWaveFormer();
	QObject::connect(mWaveFormer, SIGNAL(finished()), this, SLOT(generateNextWave()));
	QObject::connect(mWaveFormer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
	log("Generating waves.");
	locker.unlock();
	generateNextWave();
	return true;
}

void ViAudioObject::generateNextWave()
{
	QMutexLocker locker(&mMutex);

	ViAudio::Type type = mWaveFormer->type();
	if(type != ViAudio::Undefined)
	{
		locker.unlock();
		clearWaves(type);
		locker.relock();
		mWaveForms[mWaveFormer->type()] = mWaveFormer->takeWave();
		mWaveFormer->clear();
	}

	if(mWaveInstructions.isEmpty())
	{
		log("Waves generated.");
		if(mWaveFormer != NULL)
		{
			QObject::disconnect(mWaveFormer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
			QObject::disconnect(mWaveFormer, SIGNAL(finished()), this, SLOT(generateNextWave()));
			delete mWaveFormer;
			mWaveFormer = NULL;	
		}
		locker.unlock();
		setFinished();
		emit waved();
	}
	else
	{
		ViAudio::Type type = mWaveInstructions.takeFirst();
		locker.unlock();
		mWaveFormer->process(thisPointer, type);
	}
}

ViWaveForm* ViAudioObject::wave(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mWaveForms.value(type, NULL);
}

bool ViAudioObject::hasWave(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mWaveForms.contains(type);
}

void ViAudioObject::clearWaves(ViAudio::Type types)
{
	QMutexLocker locker(&mMutex);
	QList<ViAudio::Type> instructions = ViAudio::types(types);
	for(int i = 0; i < instructions.size(); ++i)
	{
		locker.unlock();
		bool hasTheWave = hasWave(instructions[i]);
		locker.relock();
		if(hasTheWave)
		{
			delete mWaveForms.take(instructions[i]);
		}
	}
}

/*******************************************************************************************************************

	FREQUENCY SPECTRUM

*******************************************************************************************************************/

bool ViAudioObject::generateSpectrum(ViAudio::Type types, const bool &force)
{

}

bool ViAudioObject::generateSpectrum(ViAudio::Type types, const int &windowSizeSamples, const QString &windowFunction, const bool &force)
{
	setStarted();
	QMutexLocker locker(&mMutex);

	mSpectrumInstructions.clear();
	QList<ViAudio::Type> instructions;
	QList<ViAudio::Type> decomposeInstructions = ViAudio::types(types);

	for(int i = 0; i < decomposeInstructions.size(); ++i)
	{
		locker.unlock();
		bool hasTheBuffer = hasBuffer(decomposeInstructions[i]);
		locker.relock();
		if(hasTheBuffer) instructions.append(decomposeInstructions[i]);
	}

	if(instructions.size() == 0)
	{
		log("No data available to generate the frequency spectrum.");
		locker.unlock();
		setFinished();
		emit spectrumed();
		return false;
	}

	if(!force)
	{
		for(int i = 0; i < instructions.size(); ++i)
		{
			locker.unlock();
			bool hasTheSpectrum = hasSpectrum(instructions[i]);
			locker.relock();
			if(!hasTheSpectrum) mSpectrumInstructions.append(instructions[i]);
		}
		if(mSpectrumInstructions.size() == 0)
		{
			locker.unlock();
			setFinished();
			emit spectrumed();
			return false;
		}
	}
	else
	{
		mSpectrumInstructions = instructions;
	}
	setProgress(mSpectrumInstructions.size());

	if(mSpectrumAnalyzer != NULL)
	{
		delete mSpectrumAnalyzer;
	}
	mSpectrumAnalyzer = new ViSpectrumAnalyzer();
	mSpectrumAnalyzer->setWindowSize(windowSizeSamples);
	mSpectrumAnalyzer->setWindowFunction(windowFunction);
	QObject::connect(mSpectrumAnalyzer, SIGNAL(finished()), this, SLOT(generateNextSpectrum()));
	QObject::connect(mSpectrumAnalyzer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
	log("Generating frequency spectrums.");
	locker.unlock();
	generateNextSpectrum();
	return true;
}

void ViAudioObject::generateNextSpectrum()
{
	QMutexLocker locker(&mMutex);

	ViAudio::Type type = mSpectrumAnalyzer->type();
	if(type != ViAudio::Undefined)
	{
		locker.unlock();
		clearSpectrums(type);
		locker.relock();
		mSpectrums[mSpectrumAnalyzer->type()] = mSpectrumAnalyzer->takeSpectrum();
		mSpectrumAnalyzer->clear();
	}

	if(mSpectrumInstructions.isEmpty())
	{
		log("Frequency spectrums generated.");
		if(mSpectrumAnalyzer != NULL)
		{
			QObject::disconnect(mSpectrumAnalyzer, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
			QObject::disconnect(mSpectrumAnalyzer, SIGNAL(finished()), this, SLOT(generateNextSpectrum()));
			delete mSpectrumAnalyzer;
			mSpectrumAnalyzer = NULL;
		}
		locker.unlock();
		setFinished();
		emit spectrumed();
	}
	else
	{
		ViAudio::Type type = mSpectrumInstructions.takeFirst();
		locker.unlock();
		mSpectrumAnalyzer->process(thisPointer, type);
	}
}

ViRealSpectrum* ViAudioObject::spectrum(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mSpectrums.value(type, NULL);
}

bool ViAudioObject::hasSpectrum(ViAudio::Type type)
{
	QMutexLocker locker(&mMutex);
	return mSpectrums.contains(type);
}

void ViAudioObject::clearSpectrums(ViAudio::Type types)
{
	QMutexLocker locker(&mMutex);
	QList<ViAudio::Type> instructions = ViAudio::types(types);
	for(int i = 0; i < instructions.size(); ++i)
	{
		locker.unlock();
		bool hasTheSpectrum = hasSpectrum(instructions[i]);
		locker.relock();
		if(hasTheSpectrum)
		{
			delete mSpectrums.take(instructions[i]);
		}
	}
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
		setFinished();
		emit corrected();
		return false;
	}
	else if(!hasBuffer(ViAudio::Corrupted))
	{
		log("No corrupted buffer is available for correction.", QtWarningMsg);
		setFinished();
		emit corrected();
		return false;
	}

	logStatus("Correcting track.");

	ViAudioFormat theFormat = format(ViAudio::Corrupted);
	buffer(ViAudio::Corrected)->setFormat(theFormat);
	buffer(ViAudio::Noise)->setFormat(theFormat);
	buffer(ViAudio::NoiseMask)->setFormat(theFormat);

	mCorrector->process(thisPointer, ViAudio::Corrupted, ViAudio::Corrected);
	return true;
}

void ViAudioObject::endCorrect()
{
	log("Track corrected.");
	setFinished();
	emit corrected();
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

void ViAudioObject::addCorrelators(QList<ViCorrelator*> correlators)
{
	for(int i = 0; i < correlators.size(); ++i)
	{
		addCorrelator(correlators[i]);
	}
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
		setFinished();
        emit correlated();;
        return false;
    }

    mCorrelations.clear();
    mCorrelationTypes.clear();
    if(hasBuffer(ViAudio::Target) && hasBuffer(ViAudio::Corrected))
	{
		mCorrelationTypes.append(QPair<ViAudio::Type, ViAudio::Type>(ViAudio::Target, ViAudio::Corrected));
    }
    if(hasBuffer(ViAudio::Target) && hasBuffer(ViAudio::Corrupted))
	{
		mCorrelationTypes.append(QPair<ViAudio::Type, ViAudio::Type>(ViAudio::Target, ViAudio::Corrupted));
    }

    if(mCorrelationTypes.isEmpty())
    {
        log("No buffers available for correlation", QtCriticalMsg);
		setFinished();
        emit correlated();
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
    }

    if(mCurrentCorrelation >= mCorrelationTypes.size())
    {
        log("The track was correlated.");
		emit changed();
		setFinished();
        emit correlated();
        return;
    }

    QPair<ViAudio::Type, ViAudio::Type> types = mCorrelationTypes[mCurrentCorrelation];
	ViCorrelator *correlator = mCorrelators[mCurrentCorrelator];

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

	correlator->process(thisPointer, types.first, types.second);
}

/*******************************************************************************************************************

	Noise MASK

*******************************************************************************************************************/

bool ViAudioObject::generateNoiseMask(ViNoiseDetector *detector)
{
	/*if(!hasBuffer(ViAudio::Corrupted))
	{
		log("A noise mask requires a corrupted signal.", QtCriticalMsg);
		setFinished();
		return false;
	}

	if(mNoiseDetector != NULL) delete mNoiseDetector;
	mNoiseDetector = detector->clone(); // We have to clone because we want to pass the same pointer (this function's paramter) to multiple objects (which can cause problems if we delete it - destructor)

	QObject::connect(mNoiseDetector, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
	QObject::connect(mNoiseDetector, SIGNAL(finished()), this, SLOT(setFinished()));
	QObject::connect(mNoiseDetector, SIGNAL(finished()), this, SIGNAL(noiseGenerated()));

	//log("Creating noise mask");
	ViAudioFormat theFormat = format(ViAudio::Corrupted);
	buffer(ViAudio::Noise)->setFormat(theFormat);
	buffer(ViAudio::NoiseMask)->setFormat(theFormat);
	mNoiseDetector->setBuffers(buffer(ViAudio::Corrupted), buffer(ViAudio::Noise), buffer(ViAudio::NoiseMask));
	mNoiseDetector->generate();*/
}

/*******************************************************************************************************************

	CUSTOM MASK

*******************************************************************************************************************/

bool ViAudioObject::generateCustomMask()
{
	if(!hasBuffer(ViAudio::Target) || !hasBuffer(ViAudio::Corrupted))
	{
		log("A custom mask requires a target and corrupted signal.", QtCriticalMsg);
		setFinished();
		return false;
	}

	log("Creating custom mask");
	ViAudioFormat theFormat = format(ViAudio::Target);
	buffer(ViAudio::Custom)->setFormat(theFormat);
	buffer(ViAudio::CustomMask)->setFormat(theFormat);
	mCustomMaskCreator->setBuffers(buffer(ViAudio::Target), buffer(ViAudio::Corrupted), buffer(ViAudio::Custom), buffer(ViAudio::CustomMask));
	mCustomMaskCreator->create();
}

/*******************************************************************************************************************

	SONG INFO

*******************************************************************************************************************/

ViMetadata& ViAudioObject::metadata()
{
	QMutexLocker locker(&mMutex);
	return mMetadata;
}

void ViAudioObject::setMetadata(const ViMetadata &metadata)
{
	QMutexLocker locker(&mMutex);
	mMetadata = metadata;
}

void ViAudioObject::detectMetadata(bool force)
{
	setStarted();
	if(!force && hasMetadata())
	{
		setFinished();
		emit metadataDetected(true);
	}

	QMutexLocker locker(&mMutex);
	mIsDetectingMetadata = true;
	if(mMetadataer != NULL)
	{
		delete mMetadataer;
	}
	mMetadataer = new ViMetadataer();
	QObject::connect(mMetadataer, SIGNAL(finished(bool)), this, SLOT(changeMetadata(bool)));

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
	mMetadataer->detect(buffers);
}

void ViAudioObject::changeMetadata(bool success)
{
	QMutexLocker locker(&mMutex);
	if(mMetadataer != NULL)
	{
		if(success)
		{
			mMetadata = mMetadataer->metadata();
		}
		delete mMetadataer;
		mMetadataer = NULL;
	}
	else
	{
		success = false;
	}

	locker.unlock();
	mIsDetectingMetadata = false;
	setFinished();
	emit metadataDetected(success);
}

bool ViAudioObject::isDetectingMetadata()
{
	QMutexLocker locker(&mMutex);
	return mIsDetectingMetadata;
}

bool ViAudioObject::hasMetadata()
{
    QMutexLocker locker(&mMutex);
	return mMetadata.isValid();
}

bool ViAudioObject::hasCover()
{
	QMutexLocker locker(&mMutex);
	return mMetadata.hasCover();
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
	ViAudio::Type type;
	foreach(type, mBuffers.keys())
	{
		if(mBuffers[type] != NULL && mBuffers[type]->streamCount(mode) > 0)
		{
			return true;
		}
	}
	return false;
}
