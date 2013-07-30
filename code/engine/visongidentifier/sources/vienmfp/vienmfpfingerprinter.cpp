#include <vienmfpfingerprinter.h>
#include <vipcmconverter.h>
#include <vilogger.h>
#include <CodegenENMFP.h>

ViEnmfpFingerprinterThread::ViEnmfpFingerprinterThread()
	: ViFingerprinterThread()
{
}

void ViEnmfpFingerprinterThread::run()
{
	ViAudioFormat format = mBufferOffset.buffer()->format();
	if(format.sampleSize() != 16)
	{
		LOG("The fingerprint calculator works only with 16bit samples.", QtCriticalMsg);
		return;
	}
	int size = mStream->size();
	setDuration(ViAudioPosition(size, ViAudioPosition::Bytes, format));
	if(size > 0)
	{
		char *chunk = new char[size];
		size = mStream->read(chunk, size);
		float *samples = new float[size / 2];
		size = ViPcmConverter<float>::pcmToReal16(chunk, samples, size);
		delete chunk;
		Codegen codegen(samples, size, 0);
		setFingerprint(QString::fromStdString(codegen.getCodeString()));
		delete samples;
	}
	mBufferOffset.clear();
}

ViEnmfpFingerprinter::ViEnmfpFingerprinter()
	: ViFingerprinter()
{
	float dummy[2];
	mVersion = QString::number(Codegen(dummy, 2, 0).getVersion());
	setThread(new ViEnmfpFingerprinterThread());
	mCoder = NULL;
}

ViEnmfpFingerprinter::~ViEnmfpFingerprinter()
{
	clean();
}

void ViEnmfpFingerprinter::generate()
{
	ViBufferOffsets temp(&mOutput);
	thread()->setBufferOffset(temp);
	thread()->generate();
}

QString ViEnmfpFingerprinter::version()
{
	return mVersion;
}

void ViEnmfpFingerprinter::generate(ViBufferOffsets bufferOffset)
{
	clean();
	mCoder = new ViAudioCoder();
	QObject::connect(mCoder, SIGNAL(finished()), this, SLOT(generate()));
	mCoder->encode(bufferOffset.buffer(), &mOutput, encodingFormat(), bufferOffset.from(), bufferOffset.to());
}

void ViEnmfpFingerprinter::clean()
{
	mOutput.clear();
	if(mCoder != NULL)
	{
		delete mCoder;
		mCoder = NULL;
	}
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
