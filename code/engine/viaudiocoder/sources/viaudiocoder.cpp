#include <viaudiocoder.h>

ViAudioCoder::ViAudioCoder()
	: QObject()
{
	qRegisterMetaType<ViAudioFormat>("ViAudioFormat");
	qRegisterMetaType<ViCoder::Error>("ViCoder::Error");

	setError(ViCoder::NoError);
	mChain = new ViCodingChain();
	QObject::connect(mChain, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(mChain, SIGNAL(progressed(qreal)), this, SIGNAL(progressed(qreal)));
	QObject::connect(mChain, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)));
	QObject::connect(mChain, SIGNAL(formatChanged(ViAudioFormat)), this, SIGNAL(formatChanged(ViAudioFormat)));
}

ViAudioCoder::~ViAudioCoder()
{
	delete mChain;
}

void ViAudioCoder::setError(ViCoder::Error error)
{
	mError = error;
	if(mError != ViCoder::NoError)
	{
		emit failed(mError);
	}
}

ViCoder::Error ViAudioCoder::error()
{
	return mError;
}

QString ViAudioCoder::errorString()
{
	return ViCoder::errorString(mError);
}

bool ViAudioCoder::isRunning()
{
	return mChain->isRunning();
}

bool ViAudioCoder::isFinished()
{
	return mChain->isFinished();
}

void ViAudioCoder::stop()
{
	mChain->quit();
}

void ViAudioCoder::convert(QString inputFilePath, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::ConvertFileToFile);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputPath(outputFilePath);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::convert(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::ConvertDataToData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::convert(QString inputFilePath, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::ConvertFileToData);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputData(output);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::convert(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::ConvertDataToFile);
	mChain->setInputData(input);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::convert(ViBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::ConvertDataToData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputData(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::convert(ViBuffer *buffer, ViBuffer *output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	output->setFormat(outputFormat);
	mChain->setMode(ViCodingChain::ConvertDataToData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputBuffer(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::decode(QString inputFilePath, ViBuffer *buffer, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::DecodeFile);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputBuffer(buffer);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::decode(QString inputFilePath, QByteArray &output, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::DecodeFile);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputData(output);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::decode(QByteArray &input, QByteArray &output, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::DecodeData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputData(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, ViBuffer *output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	output->setFormat(outputFormat);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputBuffer(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, QString outputFilePath, ViAudioFormat outputFormat, ViMetadata metadata, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->setMetadata(metadata);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, ViMetadata metadata, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputData(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->setMetadata(metadata);
	mChain->start();
}

void ViAudioCoder::encode(ViBuffer *buffer, ViBuffer *output, ViAudioFormat outputFormat, ViMetadata metadata, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	output->setFormat(outputFormat);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputBuffer(buffer);
	mChain->setOutputBuffer(output);
	mChain->setInputFormat(buffer->formatReference());
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->setMetadata(metadata);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, ViMetadata metadata, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->setMetadata(metadata);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, ViMetadata metadata, qint64 fromOffset, qint64 toOffset)
{
	setError(ViCoder::NoError);
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->setOffsets(fromOffset, toOffset);
	mChain->setMetadata(metadata);
	mChain->start();
}

ViCoderList ViAudioCoder::coders(ViAudioManager::Mode mode)
{
	return ViAudioManager::coders(mode);
}

ViCodecList ViAudioCoder::codecs(ViAudioManager::Mode mode)
{
	return ViAudioManager::codecs(mode);
}
