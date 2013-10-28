#include <vicustommaskcreator.h>

#define THRESHOLD 0.3

ViCustomMaskCreator::ViCustomMaskCreator()
	: ViNotifier()
{
}

ViCustomMaskCreator::~ViCustomMaskCreator()
{
	clear();
}

void ViCustomMaskCreator::clear()
{
	mRead1.clear();
	mRead2.clear();
	mWrite1.clear();
	mWrite2.clear();
}

void ViCustomMaskCreator::setBuffers(ViBuffer *read1, ViBuffer *read2, ViBuffer *write1, ViBuffer *write2)
{
	clear();
	mRead1.setBuffer(read1);
	mRead2.setBuffer(read2);
	mWrite1.setBuffer(write1);
	mWrite2.setBuffer(write2);
}

void ViCustomMaskCreator::create()
{
	qint64 totalSamples = qMin(mRead1.bufferSamples(), mRead2.bufferSamples());
	qint64 currentSamples = 0;
	int i, end;
	qreal difference;

	while(mRead1.hasData() && mRead2.hasData())
	{
		mRead1.read();
		mRead2.read();

		const ViSampleChunk &samples1 = mRead1.samples();
		const ViSampleChunk &samples2 = mRead2.samples();

		end = qMin(samples1.size(), samples2.size());
		currentSamples += end;

		ViSampleChunk write1(end), write2(end);
		for(i = 0; i < end; ++i)
		{
			difference = qAbs(samples1[i] - samples2[i]);
			write1[i] = difference / 2;
			if(difference >= THRESHOLD) write2[i] = 1;
			else write2[i] = 0;
		}

		mWrite1.write(write1);
		mWrite2.write(write2);
		setProgress((currentSamples * 99.0) / totalSamples);
	}
	clear();
	setProgress(100);
	emit finished();
}
