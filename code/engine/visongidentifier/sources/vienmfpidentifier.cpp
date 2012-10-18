#include "vienmfpidentifier.h"
#include "vipcmconverter.h"
#include "CodegenENMFP.h"

void ViEnmfpCodeThread::generateCode(const QByteArray *data, int offset, QString &code, QString &version, int &codeLength)
{
	QDataStream stream(mData, QIODevice::ReadOnly);
	int size = mData->size();
	if(size > 0)
	{
		char *chunk = new char[size];
		size = stream.readRawData(chunk, size);
		float *result = new float[size / 2];
		size = ViPcmConverter<float>::pcmToReal16(chunk, result, size);
		delete [] chunk;
		Codegen codegen(result, size / 2, offset);
		delete [] result;
		code = QString::fromStdString(codegen.getCodeString());
		version = QString::number(codegen.getVersion());
	}
	else
	{
		code = "";
		version = "";
		codeLength = 0;
	}
}

ViEchoCodeThread* ViEnmfpIdentifier::codeThread()
{
	return new ViEnmfpCodeThread();
}

ViAudioFormat ViEnmfpIdentifier::encodingFormat()
{
	ViAudioFormat format;
	format.setSampleRate(22050);
	format.setSampleSize(16);
	format.setSampleType(ViAudioFormat::SignedInt);
	format.setByteOrder(ViAudioFormat::LittleEndian);
	format.setChannelCount(1);
	format.setCodec(ViAudioManager::codec("WAVE"));
	return format;
}
