#include <viacoustidfingerprinter.h>
#include <vipcmconverter.h>
#include <fingerprintcalculator.h>
#include <vilogger.h>

#define CHUNK_SIZE 4096

ViAcoustidFingerprinterThread::ViAcoustidFingerprinterThread()
	: ViFingerprinterThread()
{
}

void ViAcoustidFingerprinterThread::run()
{
	ViAudioFormat format = mStream->buffer()->format();
	ViRawChunk rawChunk(CHUNK_SIZE);
	FingerprintCalculator calculator;

	if(!calculator.start(format.sampleRate(), format.channelCount()))
	{
		LOG("The fingerprint calculator could not be initialized.", QtCriticalMsg);
		return;
	}

	int size = 0;
	do
	{
		size = mStream->read(rawChunk) / 2;
		qint16 *base = reinterpret_cast<qint16*>(rawChunk.data());
		calculator.feed(base, size);
	}
	while(size > 0);
	setFingerprint(calculator.finish());
}

ViAcoustidFingerprinter::ViAcoustidFingerprinter()
	: ViFingerprinter()
{
}

void ViAcoustidFingerprinter::generate(ViBuffer *buffer)
{
	thread()->setBuffer(buffer);
	thread()->generate();
}
