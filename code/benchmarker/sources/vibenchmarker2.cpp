#include <vibenchmarker2.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <vinearestneighbournoisedetector.h>
#include <vimahalanobisnoisedetector.h>
#include <vizscorenoisedetector.h>
#include <vimadnoisedetector.h>
#include <vipredictionnoisedetector.h>
#include <vifouriernoisedetector.h>
#include <viarmanoisedetector.h>
#include <QTextStream>

#include <iomanip>

#define AI_NOISE_SEPERATION 512
#define AI_NOISE_START_LENGTH 1
#define AI_NOISE_END_LENGTH 30

#define WINDOW_SIZE 4096
#define MASK_START 0.1
#define MASK_END 1
#define MASK_INTERVAL 0.001
#define NOISE_TYPE Direct

/*#define MASK_END 0.00005
#define MASK_INTERVAL 0.0000001*/



ViBenchMarker2::ViBenchMarker2()
{
	mCurrentObject = ViAudioObject::create();

	//	Mahalanobis
	//		Forward:		PAR()	MAT ()	SEN()	SPE()
	//		Backward:		PAR()	MAT ()	SEN()	SPE()
	//		Bidirectional:	PAR()	MAT ()	SEN()	SPE()
    //mDetector = new ViMahalanobisNoiseDetector();


	//mDetector = new ViMadNoiseDetector(); //
    //mDetector = new ViFourierNoiseDetector();
	//mDetector = new ViPredictionNoiseDetector(2);
	//mDetector = new ViZscoreNoiseDetector();
    //mDetector = new ViNearestNeighbourNoiseDetector();
	mDetector = new ViArmaNoiseDetector(ViArmaNoiseDetector::AR);

	mDetector->setDirection(ViNoiseDetector::Forward);
	//mDetector->setDirection(ViNoiseDetector::Reversed);
	//mDetector->setDirection(ViNoiseDetector::Bidirectional);
}

ViBenchMarker2::~ViBenchMarker2()
{

}

void ViBenchMarker2::benchmark()
{
    QDir dir("/home/visore/Visore Projects/Files/");
	QStringList files = dir.entryList(QDir::Files);
	for(int i = 0; i < files.size(); ++i)
	{
		mFiles.enqueue(dir.absoluteFilePath(files[i]));
	}

	QString name = mDetector->name() + "_";
	if(ViNoise::NOISE_TYPE == ViNoise::Direct) name += "Direct";
	else if(ViNoise::NOISE_TYPE == ViNoise::Mean) name += "Mean";
	else if(ViNoise::NOISE_TYPE == ViNoise::Maximum) name += "Maximum";

    mOutputFile.setFileName("/home/visore/Visore Projects/Results/"+name+"_"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+".txt");
	mOutputFile.open(QIODevice::WriteOnly);
	mOutputStream.setDevice(&mOutputFile);

	mOutputStream<<name<<"\n\n\n";

	nextFile();
}

void ViBenchMarker2::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
	{
		mCurrentFile = mFiles.dequeue();

		mCurrentObject->clearBuffers();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker2::process1()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
	generateNoise();
    QObject::connect(mCurrentObject.data(), SIGNAL(noiseGenerated()), this, SLOT(process2()));
    QObject::connect(mCurrentObject.data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
	mCurrentObject->generateNoiseMask(mDetector);
}

void ViBenchMarker2::process2()
{
	//QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
	mCurrentObject->encode(ViAudio::Noise /*| ViAudio::Corrupted*/);

    //QObject::disconnect(mCurrentObject.data(), SIGNAL(noiseGenerated()), this, SLOT(process2()));

	/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
	mCurrentObject->encode();
	return;*/

	cout << mCurrentObject->filePath(ViAudio::Target).toLatin1().data() << endl;
	mOutputStream<<"\n"<<mCurrentObject->filePath(ViAudio::Target)<<"\n";
	mOutputStream.flush();

	ViAudioReadData mask(mCurrentObject->buffer(ViAudio::Noise));
	mask.setSampleCount(WINDOW_SIZE);
	ViSampleChunk *nData1 = new ViSampleChunk(mask.bufferSamples());
	ViSampleChunk *nData2 = new ViSampleChunk(mask.bufferSamples());
	ViSampleChunk *nMask1 = new ViSampleChunk(mask.bufferSamples());
	ViSampleChunk *nMask2 = new ViSampleChunk(mask.bufferSamples());

	qint64 i;
	qint64 offset1 = 0, offset2 = 0;
	qreal maxMath = 0;
	while(mask.hasData())
	{
		mask.read();
		ViSampleChunk &mask1 = mask.splitSamples(0);
		ViSampleChunk &mask2 = mask.splitSamples(1);

		for(i = 0; i < mask1.size(); ++i) (*nData1)[i + offset1] = mask1[i];
		offset1 += mask1.size();
		for(i = 0; i < mask2.size(); ++i) (*nData2)[i + offset2] = mask2[i];
		offset2 += mask2.size();
	}

	ViNoise noise1(nData1, nMask1, mCurrentThreshold);
	ViNoise noise2(nData2, nMask2, mCurrentThreshold);

	for(mCurrentThreshold = MASK_START; mCurrentThreshold <= MASK_END; mCurrentThreshold += MASK_INTERVAL)
	{
		noise1.setThreshold(mCurrentThreshold);
		noise1.generateMask(ViNoise::NOISE_TYPE);
		noise2.setThreshold(mCurrentThreshold);
		noise2.generateMask(ViNoise::NOISE_TYPE);

		qint64 truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0;

		for(i = 0; i < mNoise1.size(); ++i)
		{
			if(mNoise1[i] == 1)
			{
				if((*nMask1)[i] == 1) ++truePositives;
				else ++falseNegatives;
			}
			else if(mNoise1[i] == 0)
			{
				if((*nMask1)[i] == 1) ++falsePositives;
				else ++trueNegatives;
			}
		}

		for(i = 0; i < mNoise2.size(); ++i)
		{
			if(mNoise2[i] == 1)
			{
				if((*nMask2)[i] == 1) ++truePositives;
				else ++falseNegatives;
			}
			else if(mNoise2[i] == 0)
			{
				if((*nMask2)[i] == 1) ++falsePositives;
				else ++trueNegatives;
			}
		}

		qreal math = qSqrt(truePositives + falsePositives) * qSqrt(truePositives + falseNegatives) * qSqrt(trueNegatives + falsePositives) * qSqrt(trueNegatives + falseNegatives);
		if(math != 0)
		{
			math = ((truePositives * trueNegatives) - (falsePositives * falseNegatives)) / math;
			if(math > maxMath) maxMath = math;
		}

		mOutputStream << "\t" << mCurrentThreshold << "\t" << truePositives << "\t" << trueNegatives << "\t" << falsePositives << "\t" << falseNegatives << "\t";
		mOutputStream << truePositives / qreal(truePositives + falseNegatives) << "\t";
		mOutputStream  << trueNegatives / qreal(trueNegatives + falsePositives) << "\t";
		mOutputStream  << math << "\n";
		mOutputStream.flush();

		cout << "\t" << mCurrentThreshold << "\t" << truePositives << "\t" << trueNegatives << "\t" << falsePositives << "\t" << falseNegatives << "\t";
		cout << setprecision(10) << truePositives / qreal(truePositives + falseNegatives) << "\t";
		cout << setprecision(10) << trueNegatives / qreal(trueNegatives + falsePositives) << "\t";
		cout << math << "\t";
		cout << "(" << maxMath << ")" << endl;

	}
    mCurrentObject->clearBuffers();

	nextFile();
}

void ViBenchMarker2::generateNoise()
{
	mNoiseType = 0;
	mNoiseLength = AI_NOISE_START_LENGTH;
	mNoiseCount = AI_NOISE_END_LENGTH - mNoiseLength; // Linear add less noise as the size increases

	ViBuffer *buffer = mCurrentObject->buffer(ViAudio::Target);
	ViAudioReadData data(buffer);
	mCurrentObject->buffer(ViAudio::Corrupted)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
    ViAudioWriteData data2(mCurrentObject->buffer(ViAudio::Corrupted));

	mNoise1.clear();
	mNoise2.clear();
	mNoise1.resize(data.bufferSamples() / 2);
	mNoise2.resize(data.bufferSamples() / 2);
	qint64 offset1 = 0, offset2 = 0;
	for(qint64 i = 0; i < mNoise1.size(); ++i)
	{
		mNoise1[i] = 0;
		mNoise2[i] = 0;
	}

	while(data.hasData())
	{
		data.read();

		ViSampleChunk &c1 = data.splitSamples(0);
        if(c1.size() < 1024) break;

		int index = 0;
		while(index < c1.size())
		{
			index += AI_NOISE_SEPERATION;
			int length = addNoise(c1, index);

			offset1 += AI_NOISE_SEPERATION;
			if(offset1+length <= mNoise1.size())
			{
				for(int i = 0; i < length; ++i) mNoise1[offset1+i] = 1;
				offset1 += AI_NOISE_SEPERATION;
			}

			index += AI_NOISE_SEPERATION;
		}
		data2.enqueueSplitSamples(c1, 0);

		ViSampleChunk &c2 = data.splitSamples(1);
		index = 0;
		while(index < c2.size())
		{
			index += AI_NOISE_SEPERATION;
			int length = addNoise(c2, index);

			offset2 += AI_NOISE_SEPERATION;
			if(offset2+length <= mNoise2.size())
			{
				for(int i = 0; i < length; ++i) mNoise1[offset2+i] = 1;
				offset2 += AI_NOISE_SEPERATION;
			}

			index += AI_NOISE_SEPERATION;
		}
		data2.enqueueSplitSamples(c2, 1);
	}

	cout << "Artifical noise generated" << endl;
}

int ViBenchMarker2::addNoise(ViSampleChunk &s, int offset)
{
	if(mNoiseLength > s.size()) return 0;

	if(mNoiseType == 0)
	{
		addNoise1(s, offset, mNoiseLength);
	}
	else if(mNoiseType == 1)
	{
		addNoise2(s, offset, mNoiseLength);
	}
	else if(mNoiseType == 2)
	{
		addNoise3(s, offset, mNoiseLength);
	}
	else
	{
		cout << "INVALID NOISE LENGTH +++++++++++++" << endl;
	}

	int oldLength = mNoiseLength;

	--mNoiseCount;
	if(mNoiseCount <= 0)
	{
		++mNoiseType;
		if(mNoiseType > 2)
		{
			mNoiseType = 0;
			++mNoiseLength;
			if(mNoiseLength > AI_NOISE_END_LENGTH)
			{
				mNoiseLength = AI_NOISE_START_LENGTH;
			}
		}
		mNoiseCount = AI_NOISE_END_LENGTH - mNoiseLength;
	}

	return oldLength;
}

void ViBenchMarker2::addNoise1(ViSampleChunk &s, int offset, int length)
{
	qreal startval = 0.95;
	qreal val = startval;
	bool plus = true;
	for(int i = 0; i < length; ++i)
	{
		s[offset + i] = val;

		if(plus) val += 0.005;
		else val -= 0.005;

		if(val > 1)
		{
			val = 1;
			plus = false;
		}
		else if(val < startval)
		{
			val = startval;
			plus = true;
		}
	}
}

void ViBenchMarker2::addNoise2(ViSampleChunk &s, int offset, int length)
{
	qreal startval = -0.95;
	qreal val = startval;
	bool minus = true;
	for(int i = 0; i < length; ++i)
	{
		s[offset + i] = val;

		if(minus) val -= 0.005;
		else val += 0.005;

		if(val < -1)
		{
			val = -1;
			minus = false;
		}
		else if(val > startval)
		{
			val = startval;
			minus = true;
		}
	}
}

void ViBenchMarker2::addNoise3(ViSampleChunk &s, int offset, int length)
{
	qreal zag = 1;
	bool minus = true;
	for(int i = 0; i < length; ++i)
	{
		s[offset + i] = zag;

		if(zag == -1) minus = false;
		else if (zag == 1) minus = true;

		if(minus) zag -= 0.2;
		else zag += 0.2;
	}
}

void ViBenchMarker2::progress(qreal i)
{
    cout<<i<<" %"<<endl;
}

void ViBenchMarker2::quit()
{
	cout << "QUIT!" << endl;
	int x = 0;
	int y = 1 / x;
}
