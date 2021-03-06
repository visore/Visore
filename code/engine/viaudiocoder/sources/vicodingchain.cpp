#include <vicodingchain.h>
#include <viaudiomanager.h>
#include <vilogger.h>
#include <QFile>

ViCodingChain::ViCodingChain()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
	setOffsets(-1, -1);
	setMode(ViCodingChain::Unknown);
}

void ViCodingChain::setMode(ViCodingChain::Mode mode)
{
	reset();
	mMode = mode;
}

void ViCodingChain::setMetadata(ViMetadata metadata)
{
	mMetadata = metadata;
}

void ViCodingChain::reset()
{
	mError = ViCoder::NoError;

	if(mInput != NULL)
	{
		mInput->disconnect();
	}
	mDecoder.disconnect();
	mEncoder.disconnect();
	if(mOutput != NULL)
	{
		mOutput->disconnect();
	}

	mInputFilePath = "";
	mInputData = NULL;
	mInput = NULL;
	mOutput = NULL;
	mInputCoder = NULL;
	mOutputCoder = NULL;

	setOffsets(-1, -1);
}

void ViCodingChain::setError(ViCoder::Error error)
{
	mError = error;
	if(mInput != NULL)
	{
		mInput->disconnect();
	}
	mDecoder.disconnect();
	mEncoder.disconnect();
	if(mOutput != NULL)
	{
		mOutput->disconnect();
	}

	if(isRunning())
	{
		exit(-1);
	}

	if(mInputCoder != NULL)
	{
		mInputCoder->unload();
	}
	if(mOutputCoder != NULL)
	{
		mOutputCoder->unload();
	}

	if(mError != ViCoder::NoError)
	{
		emit failed(mError);
	}
}

void ViCodingChain::setInputPath(QString filePath)
{
	mInputFilePath = filePath;
	mFileInput.setFilePath(filePath);
	mInput = &mFileInput;
	detectCoder = &ViCodingChain::detectCoderFile;
}

void ViCodingChain::setOutputPath(QString filePath)
{
	mFileOutput.setFilePath(filePath);
	mOutput = &mFileOutput;
}

void ViCodingChain::setInputData(QByteArray &array)
{
	mInputData = &array;
	mDataInput.setData(array);
	mInput = &mDataInput;
	detectCoder = &ViCodingChain::detectCoderData;
}

void ViCodingChain::setOutputData(QByteArray &array)
{
	mDataOutput.setData(array);
	mOutput = &mDataOutput;
}

void ViCodingChain::setInputBuffer(ViBuffer *buffer)
{
	mBufferInput.setBuffer(buffer);
	mInput = &mBufferInput;
}

void ViCodingChain::setOutputBuffer(ViBuffer *buffer)
{
	mBufferOutput.setBuffer(buffer);
	mOutput = &mBufferOutput;
}

void ViCodingChain::setInputFormat(ViAudioFormat format)
{
	mInputFormat = format;
	if(mInputFormat.sampleSize() <= 0)
	{
		LOG("The input format\'s sample size is not valid.", QtCriticalMsg);
	}
}

void ViCodingChain::setOutputFormat(ViAudioFormat format)
{
	mOutputFormat = format;
	if(mOutputFormat.sampleSize() <= 0)
	{
		LOG("The output format\'s sample size is not valid.", QtCriticalMsg);
	}
}

void ViCodingChain::detectCoderData()
{
	mInputCoder = ViAudioManager::detect(*mInputData);
}

void ViCodingChain::detectCoderFile()
{
	mInputCoder = ViAudioManager::detect(mInputFilePath);
}

void ViCodingChain::setOffsets(qint64 fromOffset, qint64 toOffset)
{
	mFromOffset = fromOffset;
	mToOffset = toOffset;
}

void ViCodingChain::run()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Initialize
	//
	//////////////////////////////////////////////////////////////////////////////////////////////

	emit progressed(0);

	if(mMode == ViCodingChain::Unknown)
	{
		return;
	}

	if(mMode != ViCodingChain::EncodeFile && mMode != ViCodingChain::EncodeData)
	{
		(this->*detectCoder)();
		if(mInputCoder == NULL)
		{
			ViCoder::Error error = ViAudioManager::error();
			if(error == ViCoder::UnsupportedCodecError)
			{
				setError(ViCoder::UnsupportedInputCodecError);
			}
			else if(error == ViCoder::UnavailableCodecError)
			{
				setError(ViCoder::UnavailableInputCodecError);
			}
			else
			{
				setError(error);
			}
			return;
		}
		else
		{
			QObject::connect(mInputCoder, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);
		}
	}
	if(mError != ViCoder::NoError) return;

	if(mMode != ViCodingChain::DecodeFile && mMode != ViCodingChain::DecodeData)
	{
		mOutputCoder = ViAudioManager::coder(mOutputFormat);
		if(mOutputCoder == NULL)
		{
			ViCoder::Error error = ViAudioManager::error();
			if(error == ViCoder::UnsupportedCodecError)
			{
				setError(ViCoder::UnsupportedOutputCodecError);
			}
			else if(error == ViCoder::UnavailableCodecError)
			{
				setError(ViCoder::UnavailableOutputCodecError);
			}
			else
			{
				setError(error);
			}
			return;
		}
		else
		{
			mOutputCoder->setMetadata(mMetadata);
			QObject::connect(mOutputCoder, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);
		}
	}
	if(mError != ViCoder::NoError) return;

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Initialize mode
	//
	//////////////////////////////////////////////////////////////////////////////////////////////

	mInput->disconnect();
	mDecoder.disconnect();
	mEncoder.disconnect();
	mOutput->disconnect();
	QObject::connect(mInput, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);
	QObject::connect(&mDecoder, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);
	QObject::connect(&mEncoder, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);
	QObject::connect(mOutput, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)), Qt::DirectConnection);

	if(mMode == ViCodingChain::ConvertFileToFile || mMode == ViCodingChain::ConvertFileToData || mMode == ViCodingChain::ConvertDataToFile || mMode == ViCodingChain::ConvertDataToData)
	{
		mInput->setNext(&mDecoder);
		mDecoder.setNext(&mEncoder);
		mEncoder.setNext(mOutput);
		mOutputCoder->setFormat(ViAudio::AudioOutput, mOutputFormat);
		if(mMode == ViCodingChain::ConvertDataToFile || mMode == ViCodingChain::ConvertDataToData)
		{
			mInputCoder->setFormat(ViAudio::AudioInput, mInputFormat);
		}
	}
	else if(mMode == ViCodingChain::DecodeFile || mMode == ViCodingChain::DecodeData)
	{
		mInput->setNext(&mDecoder);
		mDecoder.setNext(mOutput);
		if(mMode == ViCodingChain::DecodeData)
		{
			mInputCoder->setFormat(ViAudio::AudioInput, mInputFormat);
		}
	}
	else if(mMode == ViCodingChain::EncodeFile || mMode == ViCodingChain::EncodeData)
	{
		mInput->setNext(&mEncoder);
		mEncoder.setNext(mOutput);
		mOutputCoder->setFormat(ViAudio::AudioOutput, mOutputFormat);
	}

	mInput->setOffsets(mFromOffset, mToOffset);
	mInput->initialize();
	if(mError != ViCoder::NoError) return;

	mOutput->initialize();
	if(mError != ViCoder::NoError) return;
	if(mMode != ViCodingChain::EncodeFile && mMode != ViCodingChain::EncodeData)
	{
		mDecoder.setCoder(mInputCoder);
		QObject::connect(mInputCoder, SIGNAL(formatChanged(ViAudioFormat)), this, SIGNAL(formatChanged(ViAudioFormat)), Qt::UniqueConnection);
		mInputCoder->load();
		if(mError != ViCoder::NoError) return;
		mDecoder.initialize();
		if(mError != ViCoder::NoError) return;
	}
	if(mMode != ViCodingChain::DecodeFile && mMode != ViCodingChain::DecodeData)
	{
		mEncoder.setCoder(mOutputCoder);
		mOutputCoder->load();
		if(mError != ViCoder::NoError) return;
		mEncoder.initialize();
		if(mError != ViCoder::NoError) return;
		if(mMode == ViCodingChain::EncodeFile || mMode == ViCodingChain::EncodeData)
		{
			mOutputCoder->setFormat(ViAudio::AudioInput, mInputFormat);
			mInput->setSampleSize(mInputFormat.sampleSize());
			mEncoder.changeFormat(mInputFormat);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Execute
	//
	//////////////////////////////////////////////////////////////////////////////////////////////

	qreal progressedData = 0;
	qint64 totalSize = mInput->size();
	static qint32 chunkSize = mInput->chunkSize();
	qreal progress = 0;

	while(mInput->hasData() && mError == ViCoder::NoError)
	{
		mInput->execute();
		progressedData += chunkSize;
		progress = progressedData / totalSize * 99; // * 99 to first finalize everything before 100% is emitted
		emit progressed(progress);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Finalize
	//
	//////////////////////////////////////////////////////////////////////////////////////////////

	mInput->finalize();
	if(mError != ViCoder::NoError) return;
	if(mMode != ViCodingChain::EncodeFile && mMode != ViCodingChain::EncodeData)
	{
		if(mMode == ViCodingChain::DecodeFile || mMode == ViCodingChain::DecodeData)
		{
			mInputFormat = mInputCoder->format(ViAudio::AudioInput);
		}
		mDecoder.finalize();
		if(mError != ViCoder::NoError) return;
	}
	if(mMode != ViCodingChain::DecodeFile && mMode != ViCodingChain::DecodeData)
	{
		mEncoder.finalize();
		if(mError != ViCoder::NoError) return;
	}
	mOutput->finalize();

	if(mInputCoder != NULL)
	{
		mInputCoder->unload();
	}
	if(mOutputCoder != NULL)
	{
		mOutputCoder->unload();
	}

	progress = 100;
	emit progressed(progress);
}
