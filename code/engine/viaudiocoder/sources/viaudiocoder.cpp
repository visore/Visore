#include <viaudiocoder.h>

ViAudioCoder::ViAudioCoder()
	: QObject()
{
	qRegisterMetaType<ViAudioFormat>("ViAudioFormat");
	qRegisterMetaType<ViCoder::Error>("ViCoder::Error");

	mChain = new ViCodingChain();
	QObject::connect(mChain, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(mChain, SIGNAL(progressed(qreal)), this, SIGNAL(progressed(qreal)));
	QObject::connect(mChain, SIGNAL(failed(ViCoder::Error)), this, SLOT(setError(ViCoder::Error)));
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

void ViAudioCoder::convert(QString inputFilePath, QString outputFilePath, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::ConvertFileToFile);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputPath(outputFilePath);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

void ViAudioCoder::convert(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::ConvertDataToData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

void ViAudioCoder::convert(QString inputFilePath, QByteArray &output, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::ConvertFileToData);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputData(output);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

void ViAudioCoder::convert(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::ConvertDataToFile);
	mChain->setInputData(input);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

void ViAudioCoder::decode(QString inputFilePath, QByteArray &output, ViAudioFormat &inputFormat)
{
	mChain->setMode(ViCodingChain::DecodeFile);
	mChain->setInputPath(inputFilePath);
	mChain->setOutputData(output);
	mChain->setInputFormat(&inputFormat);
	mChain->start();
}

void ViAudioCoder::decode(QByteArray &input, QByteArray &output, ViAudioFormat &inputFormat)
{
	mChain->setMode(ViCodingChain::DecodeData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(&inputFormat);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputPath(outputFilePath);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

void ViAudioCoder::encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat)
{
	mChain->setMode(ViCodingChain::EncodeData);
	mChain->setInputData(input);
	mChain->setOutputData(output);
	mChain->setInputFormat(inputFormat);
	mChain->setOutputFormat(outputFormat);
	mChain->start();
}

ViCoderList ViAudioCoder::coders(ViAudioManager::Mode mode)
{
	return ViAudioManager::instance().coders(mode);
}

ViCodecList ViAudioCoder::codecs(ViAudioManager::Mode mode)
{
	return ViAudioManager::instance().codecs(mode);
}
