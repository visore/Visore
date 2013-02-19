#include <vienmfpfingerprinter.h>
#include <vipcmconverter.h>
#include <CodegenENMFP.h>

ViEnmfpFingerprinterThread::ViEnmfpFingerprinterThread()
	: ViFingerprinterThread()
{
}

void ViEnmfpFingerprinterThread::run()
{
	int size = mStream->size();
	if(size > 0)
	{
		ViRawChunk chunk(size);
		size = mStream->read(chunk);
		ViChunk<qfloat> sampleChunk(size / 2);
		size = ViPcmConverter<float>::pcmToReal16(chunk.data(), sampleChunk.data(), size);
		Codegen codegen(sampleChunk.data(), size / 2, 0);
		setFingerprint(QString::fromStdString(codegen.getCodeString()));
	}
}

ViEnmfpFingerprinter::ViEnmfpFingerprinter()
	: ViFingerprinter()
{
	float dummy[2];
	mVersion = QString::number(Codegen(dummy, 2, 0).getVersion());
	setThread(new ViEnmfpFingerprinterThread());
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(generate()));
}

void ViEnmfpFingerprinter::generate()
{
	thread()->setBuffer(&mOutput);
	thread()->generate();
}

QString ViEnmfpFingerprinter::version()
{
	return mVersion;
}

void ViEnmfpFingerprinter::generate(ViBuffer *buffer)
{
	mOutput.clear();
	mCoder.encode(buffer, &mOutput, encodingFormat());
}

ViAudioFormat ViEnmfpFingerprinter::encodingFormat()
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
