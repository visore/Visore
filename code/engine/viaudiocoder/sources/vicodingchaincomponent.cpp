#include <vicodingchaincomponent.h>
#include <QFileInfo>
#include <vimanager.h>

#define CHUNK_SIZE 8192

/**********************************************************
ViCodingChainComponent
**********************************************************/

ViCodingChainComponent::ViCodingChainComponent()
	: QObject()
{
	mNext = NULL;
	mError = ViCoder::NoError;
	mContinue = true;
}

ViCoder::Error ViCodingChainComponent::error()
{
	return mError;
}

void ViCodingChainComponent::setError(ViCoder::Error error)
{
	mError = error;
	emit failed(mError);
}

void ViCodingChainComponent::setHeader(QByteArray header)
{
	mHeader = header;
}

void ViCodingChainComponent::setNext(ViCodingChainComponent *next)
{
	mNext = next;
}

int ViCodingChainComponent::chunkSize()
{
	return CHUNK_SIZE;
}

void ViCodingChainComponent::changeFormat(ViAudioFormat format)
{
}

void ViCodingChainComponent::addData(ViSampleArray *data)
{
	mData.enqueue(data);
	execute();
}

/**********************************************************
ViCodingChainInput
**********************************************************/

ViCodingChainInput::ViCodingChainInput()
	: ViCodingChainComponent()
{
	mSampleSize = -1;
	mFromOffset = -1;
	mToOffset = -1;
	mTotalReadSize = 0;
}

void ViCodingChainInput::setSampleSize(int size)
{
	mSampleSize = size / 8;
}

void ViCodingChainInput::setOffsets(qint64 from, qint64 to)
{
	mFromOffset = from;
	mToOffset = to;
}

void ViCodingChainInput::initializeOffsets(int size)
{
	mTotalReadSize = 0;

	if(mFromOffset < 0)
	{
		mFromOffset = 0;
	}
	if(mToOffset < 0 || mToOffset > size)
	{
		mToOffset = size;
	}
	if(mFromOffset > 0)
	{
		seek(mFromOffset);
	}
}

int ViCodingChainInput::readSize()
{
	if(mTotalReadSize + CHUNK_SIZE <= mToOffset)
	{
		return CHUNK_SIZE;
	}
	else
	{
		return mToOffset - mTotalReadSize;
	}
}

void ViCodingChainInput::addReadSize(const int &size)
{
	mTotalReadSize += size;
}

/**********************************************************
ViCodingChainFileInput
**********************************************************/

ViCodingChainFileInput::ViCodingChainFileInput()
	: ViCodingChainInput()
{
	mFilePath = "";
}

void ViCodingChainFileInput::seek(int position)
{
	mFile.seek(position);
}

void ViCodingChainFileInput::setFilePath(QString filePath)
{
	mFilePath = filePath;
}

bool ViCodingChainFileInput::hasData()
{
	return readSize() > 0 && !mFile.atEnd();
}

int ViCodingChainFileInput::size()
{
	return mFile.size();
}

void ViCodingChainFileInput::initialize()
{
	mData.clear();
	if(mFilePath == "")
	{
		setError(ViCoder::InputFileError);
		return;
	}
	mFile.setFileName(mFilePath);
	if(mFile.open(QIODevice::ReadOnly))
	{
		initializeOffsets(mFile.size());
	}
	else
	{
		setError(ViCoder::InputFileError);
	}
}

void ViCodingChainFileInput::execute()
{
	int size = readSize();
	if(size > 0)
	{
		char *data = new char[size];
		size = mFile.read(data, size);
		addReadSize(size);
		mNext->addData(new ViSampleArray(data, size));
	}
}

void ViCodingChainFileInput::finalize()
{
	mFile.close();
}

/**********************************************************
ViCodingChainDataInput
**********************************************************/

ViCodingChainDataInput::ViCodingChainDataInput()
	: ViCodingChainInput()
{
	mByteArray = NULL;
	mStream = NULL;
}

ViCodingChainDataInput::~ViCodingChainDataInput()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	mByteArray = NULL;
}

void ViCodingChainDataInput::seek(int position)
{
	if(mStream != NULL)
	{
		delete mStream;
	}
	mStream = new QDataStream(mByteArray, QIODevice::ReadOnly);
	mStream->skipRawData(position);
}

void ViCodingChainDataInput::setData(QByteArray &data)
{
	mByteArray = &data;
}

bool ViCodingChainDataInput::hasData()
{
	return readSize() > 0 && !mStream->atEnd();
}

int ViCodingChainDataInput::size()
{
	return mByteArray->size();
}

void ViCodingChainDataInput::initialize()
{
	mData.clear();
	if(mStream != NULL)
	{
		delete mStream;
	}
	mStream = new QDataStream(mByteArray, QIODevice::ReadOnly);
	initializeOffsets(mByteArray->size());
}

void ViCodingChainDataInput::execute()
{
	int size = readSize();
	if(size > 0)
	{
		char *data = new char[size];
		size = mStream->readRawData(data, size);
		addReadSize(size);
		mNext->addData(new ViSampleArray(data, size, size / mSampleSize));
	}
}

void ViCodingChainDataInput::finalize()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	mByteArray = NULL;
}

/**********************************************************
ViCodingChainBufferInput
**********************************************************/

ViCodingChainBufferInput::ViCodingChainBufferInput()
	: ViCodingChainInput()
{
	mBuffer = NULL;
}

ViCodingChainBufferInput::~ViCodingChainBufferInput()
{
	mBuffer = NULL;
}

void ViCodingChainBufferInput::seek(int position)
{
	mStream->setPosition(position);
}

void ViCodingChainBufferInput::setBuffer(ViBuffer *buffer)
{
	mBuffer = buffer;
}

bool ViCodingChainBufferInput::hasData()
{
	return readSize() > 0 && !mStream->atEnd();
}

int ViCodingChainBufferInput::size()
{
	return mStream->size();
}

void ViCodingChainBufferInput::initialize()
{
	mStream = mBuffer->createReadStream();
	initializeOffsets(mStream->size());
}

void ViCodingChainBufferInput::execute()
{
	int size = readSize();
	if(size > 0)
	{
		char *data = new char[size];
		size = mStream->read(data, size);
		addReadSize(size);
		mNext->addData(new ViSampleArray(data, size, size / mSampleSize));
	}
}

void ViCodingChainBufferInput::finalize()
{
	mBuffer= NULL;
	mStream = NULL;
}

/**********************************************************
ViCodingChainCoder
**********************************************************/

ViCodingChainCoder::ViCodingChainCoder()
	: ViCodingChainComponent()
{
	mCoder = NULL;
}

void ViCodingChainCoder::setCoder(ViAbstractCoder *coder)
{
	mCoder = coder;
}

/**********************************************************
ViCodingChainDecoder
**********************************************************/

ViCodingChainDecoder::ViCodingChainDecoder()
	: ViCodingChainCoder()
{
}

void ViCodingChainDecoder::setCoder(ViAbstractCoder *coder)
{
	ViCodingChainCoder::setCoder(coder);
	coder->disconnect(coder, SIGNAL(formatChanged(ViAudioFormat)));
	QObject::connect(coder, SIGNAL(formatChanged(ViAudioFormat)), mNext, SLOT(changeFormat(ViAudioFormat)), Qt::DirectConnection);
}

void ViCodingChainDecoder::initialize()
{
	mData.clear();
	if(mCoder != NULL)
	{
		if(mCoder->initializeDecode())
		{
			mCoder->disconnect(mCoder, SIGNAL(decoded(ViSampleArray*)));
			QObject::connect(mCoder, SIGNAL(decoded(ViSampleArray*)), mNext, SLOT(addData(ViSampleArray*)), Qt::DirectConnection);
		}
		else
		{
			setError(ViCoder::DecoderInitializationError);
		}
	}
}

void ViCodingChainDecoder::finalize()
{
	if(mCoder != NULL)
	{
		if(!mCoder->finalizeDecode())
		{
			setError(ViCoder::DecoderFinalizationError);
		}
		mCoder->disconnect();
	}
}

void ViCodingChainDecoder::execute()
{
	ViSampleArray *array = mData.dequeue();
	mCoder->decode(array->data(), array->size());
	delete array;
}

/**********************************************************
ViCodingChainEncoder
**********************************************************/

ViCodingChainEncoder::ViCodingChainEncoder()
	: ViCodingChainCoder()
{
}

void ViCodingChainEncoder::changeFormat(ViAudioFormat format)
{
	if(mCoder != NULL)
	{
		mCoder->setFormat(ViAudio::AudioInput, format);
		if(mCoder->initializeEncode())
		{
			mCoder->disconnect(mCoder, SIGNAL(encoded(ViSampleArray*)));
			QObject::connect(mCoder, SIGNAL(encoded(ViSampleArray*)), mNext, SLOT(addData(ViSampleArray*)), Qt::DirectConnection);
		}
		else
		{
			setError(ViCoder::EncoderInitializationError);
		}
	}
}

void ViCodingChainEncoder::initialize()
{
	mData.clear();
}

void ViCodingChainEncoder::finalize()
{
	if(mCoder != NULL)
	{
		if(!mCoder->finalizeEncode())
		{
			setError(ViCoder::EncoderFinalizationError);
		}
		mCoder->disconnect();
	}
	mNext->setHeader(mCoder->header());
}

void ViCodingChainEncoder::execute()
{
	ViSampleArray *array = mData.dequeue();
	mCoder->encode(array->data(), array->samples());
	delete array;
}


/**********************************************************
ViCodingChainOutput
**********************************************************/

ViCodingChainOutput::ViCodingChainOutput()
	: ViCodingChainComponent()
{
}

void ViCodingChainOutput::initialize()
{
}

/**********************************************************
ViCodingChainFileOutput
**********************************************************/

ViCodingChainFileOutput::ViCodingChainFileOutput()
	: ViCodingChainOutput()
{
	mFilePath = "";
}

void ViCodingChainFileOutput::setFilePath(QString filePath)
{
	mFilePath = filePath;
}

void ViCodingChainFileOutput::initialize()
{
	ViCodingChainOutput::initialize();
	mData.clear();
	if(mFilePath == "")
	{
		setError(ViCoder::OutputFileError);
		return;
	}
	QFileInfo info(mFilePath);
	QString folder = ViManager::tempPath() + "coder";
	QDir dir(folder);
	if(!dir.exists())
	{
		dir.mkpath(folder);
	}
	mTempFile.setFileName(folder + QDir::separator() + info.fileName());
	if(!mTempFile.open(QIODevice::WriteOnly))
	{
		setError(ViCoder::OutputFileError);
	}
}

void ViCodingChainFileOutput::finalize()
{
	mTempFile.close();
	mFile.setFileName(mFilePath);
	if(!mTempFile.open(QIODevice::ReadOnly) || !mFile.open(QIODevice::WriteOnly))
	{
		setError(ViCoder::OutputFileError);
	}

	mFile.write(mHeader);
	mHeader.clear();	

	char data[CHUNK_SIZE];
	qint64 size = 0;
	while(!mTempFile.atEnd())
	{
		size = mTempFile.read(data, CHUNK_SIZE);
		mFile.write(data, size);
	}
	mTempFile.close();
	mTempFile.remove();
	mFile.close();
}

void ViCodingChainFileOutput::execute()
{
	ViSampleArray *array = mData.dequeue();
	mTempFile.write(array->charData(), array->size());
	delete array;
}

/**********************************************************
ViCodingChainDataOutput
**********************************************************/

ViCodingChainDataOutput::ViCodingChainDataOutput()
	: ViCodingChainOutput()
{
	mByteArray = NULL;
	mStream = NULL;
}

ViCodingChainDataOutput::~ViCodingChainDataOutput()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	mByteArray = NULL;
}

void ViCodingChainDataOutput::setData(QByteArray &data)
{
	mByteArray = &data;
}

void ViCodingChainDataOutput::initialize()
{
	ViCodingChainOutput::initialize();
	mData.clear();
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	mStream = new QDataStream(mByteArray, QIODevice::WriteOnly);
}

void ViCodingChainDataOutput::finalize()
{
	mByteArray->insert(0, mHeader);
	mHeader.clear();
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	mByteArray = NULL;
}

void ViCodingChainDataOutput::execute()
{
	ViSampleArray *array = mData.dequeue();
	mStream->writeRawData(array->charData(), array->size());
	delete array;
}

/**********************************************************
ViCodingChainBufferOutput
**********************************************************/

ViCodingChainBufferOutput::ViCodingChainBufferOutput()
	: ViCodingChainOutput()
{
	mBuffer = NULL;
}

ViCodingChainBufferOutput::~ViCodingChainBufferOutput()
{
	mBuffer = NULL;
}

void ViCodingChainBufferOutput::changeFormat(ViAudioFormat format)
{
	if(mBuffer != NULL)
	{
		mBuffer->setFormat(format);
	}
}

void ViCodingChainBufferOutput::setBuffer(ViBuffer *buffer)
{
	mBuffer = buffer;
}

void ViCodingChainBufferOutput::initialize()
{
	ViCodingChainOutput::initialize();
	mStream = mBuffer->createWriteStream();
}

void ViCodingChainBufferOutput::finalize()
{
	mStream->insert(0, mHeader.data(), mHeader.size());
	mHeader.clear();
	mBuffer = NULL;
	mStream = NULL;
}

void ViCodingChainBufferOutput::execute()
{
	ViSampleArray *array = mData.dequeue();
	mStream->write(array->charData(), array->size());
	delete array;
}
