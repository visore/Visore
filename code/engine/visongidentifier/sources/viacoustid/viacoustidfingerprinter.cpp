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
	ViAudioFormat format = mBufferOffset.buffer()->format();
	ViRawChunk rawChunk(CHUNK_SIZE);
	FingerprintCalculator calculator;
	if(format.sampleSize() != 16)
	{
		LOG("The fingerprint calculator works only with 16bit samples.", QtCriticalMsg);
		return;
	}
	if(!calculator.start(format.sampleRate(), format.channelCount()))
	{
		LOG("The fingerprint calculator could not be initialized.", QtCriticalMsg);
		return;
	}

	int size = 0, to = mBufferOffset.to(), total = 0;
	do
	{
		size = mStream->read(rawChunk);
		total += size;
		qint16 *base = reinterpret_cast<qint16*>(rawChunk.data());
		calculator.feed(base, size/2);
	}
	while(size > 0 && total < to);

	setDuration(ViAudioPosition(mBufferOffset.buffer()->size(), ViAudioPosition::Bytes, format)); // AcoustId works better with the duration of the full song, even if the fingerprint wasn't generate with the entire track.
	setFingerprint(calculator.finish());
	mBufferOffset.clear();
}

ViAcoustidFingerprinter::ViAcoustidFingerprinter()
	: ViFingerprinter()
{
	setThread(new ViAcoustidFingerprinterThread());
}

void ViAcoustidFingerprinter::generate(ViBufferOffsets bufferOffset)
{
	thread()->setBufferOffset(bufferOffset);
	thread()->generate();
}
