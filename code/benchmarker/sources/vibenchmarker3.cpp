#include <vibenchmarker3.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <vinoisecreator.h>
#include <vinearestneighbournoisedetector.h>
#include <vimahalanobisnoisedetector.h>
#include <vizscorenoisedetector.h>
#include <vimadnoisedetector.h>
#include <vipredictionnoisedetector.h>
#include <vifouriernoisedetector.h>
#include <viarmanoisedetector.h>
#include <QTextStream>

#include <iomanip>

#define WINDOW_SIZE 4096
#define MASK_START 0
#define MASK_END 1
//#define MASK_INTERVAL 0.0001
#define MASK_INTERVAL 0.001
#define NOISE_TYPE Direct

#define NO_CHANGE 50

ViBenchMarker3::ViBenchMarker3()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	//mDetector = new ViArmaNoiseDetector(ViArmaNoiseDetector::AR, ViArmaNoiseDetector::Gretl);
	//mDetector = new ViMahalanobisNoiseDetector();
	mDetector = new ViMadNoiseDetector();
	//mDetector = new ViFourierNoiseDetector();
	//mDetector = new ViPredictionNoiseDetector(2);
	//mDetector = new ViZscoreNoiseDetector();
	//mDetector = new ViNearestNeighbourNoiseDetector();

	mDetector->setDirection(ViNoiseDetector::Forward);
	//mDetector->setDirection(ViNoiseDetector::Reversed);
	//mDetector->setDirection(ViNoiseDetector::Bidirectional);

	addParam("WINDOW SIZE", 1, 500, 1);
	//addParam("K", 65, 300, 1);
}

ViBenchMarker3::~ViBenchMarker3()
{
}

void ViBenchMarker3::addParam(QString name, qreal start, qreal end, qreal increase)
{
	mParamsName.append(name);
	mParamsStart.append(start);
	mParamsEnd.append(end);
	mParamsIncrease.append(increase);
	mParamsCurrent.append(0);
}

void ViBenchMarker3::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViBenchMarker3::nextParam()
{
	int size = mParamsStart.size();

	bool finished = true;
	for(int i = 0; i < size; ++i)
	{
		if(mParamsCurrent[i] < mParamsEnd[i])
		{
			finished = false;
			break;
		}
	}
	if(finished) return false; //All paramaters were tested

	for(int i = size - 1; i >= 0; --i)
	{
		if(mParamsCurrent[i] < mParamsEnd[i])
		{
			mParamsCurrent[i] += mParamsIncrease[i];
			return true;
		}
		else if(mParamsCurrent[i] >= mParamsEnd[i])
		{
			mParamsCurrent[i] = mParamsStart[i];
			int pre = 1;
			while(mParamsCurrent[i-pre] >= mParamsEnd[i-pre])
			{
				mParamsCurrent[i-pre] = mParamsStart[i-pre];
				++pre;
			}
			mParamsCurrent[i-pre] += mParamsIncrease[i-pre];
			return true;
		}
	}
}

void ViBenchMarker3::benchmark()
{
	QDir dir("/home/visore/Visore Projects/Files/");
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for(int i = 0; i < dirs.size(); ++i)
	{
		QDir dir3("/home/visore/Visore Projects/Results/"+dirs[i]);
		if(!dir3.exists()) dir3.mkpath(dir3.absolutePath());

		QDir dir2(dir.absoluteFilePath(dirs[i]));
		QStringList files2 = dir2.entryList(QDir::Files);
		for(int j = 0; j < files2.size(); ++j)
		{
			mFiles.enqueue(dir2.absoluteFilePath(files2[j]));
			mResults.enqueue(dir3.absoluteFilePath(files2[j])+".txt");
		}
	}
	mTotalFiles = mFiles.size();
	nextFile();
}

void ViBenchMarker3::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
	{
		initParams();
		for(int i = 0; i < mParamsStart.size(); ++i)
		{
			mParamsCurrent[i] = mParamsStart[i];
		}

		mCurrentFile = mFiles.dequeue();

		mOutputFile.close();
		mOutputFile.setFileName(mResults.dequeue());
		mOutputFile.open(QIODevice::WriteOnly);
		mOutputStream.setDevice(&mOutputFile);

		mOutputStream<<mDetector->name()<<"\n";
		if(mDetector->direction() == ViNoiseDetector::Forward) mOutputStream<< "Forward";
		else if(mDetector->direction() == ViNoiseDetector::Reversed) mOutputStream<< "Reversed";
		else if(mDetector->direction() == ViNoiseDetector::Bidirectional) mOutputStream<< "Bidirectional";
		mOutputStream<<"\n";
		if(ViNoise::NOISE_TYPE == ViNoise::Direct) mOutputStream<< "Direct";
		else if(ViNoise::NOISE_TYPE == ViNoise::Mean) mOutputStream<< "Mean";
		else if(ViNoise::NOISE_TYPE == ViNoise::Maximum) mOutputStream<< "Maximum";
		mOutputStream<<"\n\n";
		mOutputStream << QFileInfo(mCurrentFile).fileName();
		mOutputStream<<"\n\n";

		for(int i = 0; i < mParamsName.size(); ++i)
		{
			mOutputStream << "PARAMETER "<<(i+1) << " (" << mParamsName[i] << ")\t";
		}
		mOutputStream << "REAL THRESHOLD" << "\t" << "(UN)AMPLIFIED THRESHOLD" << "\t" << "TRUE POSITIVES" << "\t" << "TRUE NEGATIVES" << "\t" << "FALSE POSITIVES"<< "\t" << "FALSE NEGATIVES" << "\t";
		mOutputStream << "SENSITIVITY" << "\t" << "SPECIFISITY" << "\t";
		mOutputStream << "MATTHEWS SCORE" << "\t" << "TIME" << "\n";

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();

		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker3::process1(bool generate)
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));

	if(generate)
	{
		ViNoiseCreator creator;
		creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::CustomMask));
	}
	mCurrentObject->clearBuffer(ViAudio::Target);
	mCurrentObject->clearBuffer(ViAudio::Noise);
	mCurrentObject->clearBuffer(ViAudio::NoiseMask);

	if(mParamsStart.size() == 1) mDetector->setParameters(mParamsCurrent[0]);
	else if(mParamsStart.size() == 2) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1]);
	else if(mParamsStart.size() == 3) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2]);
	else if(mParamsStart.size() == 4) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2], mParamsCurrent[3]);
	//else { cout << "Invalid parameter count of "<<mParamsStart.size()<<". Min: 1, Max: 4" << endl; quit(); }

	QObject::connect(mCurrentObject.data(), SIGNAL(noiseGenerated()), this, SLOT(process2()));
	mTime.restart();
	mCurrentObject->generateNoiseMask(mDetector);
}

void ViBenchMarker3::process2()
{
	int time = mTime.elapsed();
	QObject::disconnect(mCurrentObject.data(), SIGNAL(noiseGenerated()), this, SLOT(process2()));

	/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
	mCurrentObject->encode(ViAudio::Noise);
	return;*/

	qreal maxMAT = 0;
	qint64 maxTP = 0, maxTN = 0, maxFP = 0, maxFN = 0;
	qint64 i, offset1 = 0, offset2 = 0, offset3 = 0, offset4 = 0;
	int noChange = 0;

	ViAudioReadData corrupted(mCurrentObject->buffer(ViAudio::Noise));
	ViAudioReadData realMask(mCurrentObject->buffer(ViAudio::CustomMask));
	corrupted.setSampleCount(WINDOW_SIZE);
	realMask.setSampleCount(WINDOW_SIZE);

	ViSampleChunk *nData1 = new ViSampleChunk(corrupted.bufferSamples() / 2);
	ViSampleChunk *nData2 = new ViSampleChunk(corrupted.bufferSamples() / 2);
	ViSampleChunk *nMask1 = new ViSampleChunk(corrupted.bufferSamples() / 2);
	ViSampleChunk *nMask2 = new ViSampleChunk(corrupted.bufferSamples() / 2);
	ViSampleChunk *nRealMask1 = new ViSampleChunk(realMask.bufferSamples() / 2);
	ViSampleChunk *nRealMask2 = new ViSampleChunk(realMask.bufferSamples() / 2);

	while(corrupted.hasData() && realMask.hasData())
	{
		corrupted.read();
		ViSampleChunk &corrupted1 = corrupted.splitSamples(0);
		ViSampleChunk &corrupted2 = corrupted.splitSamples(1);

		for(i = 0; i < corrupted1.size(); ++i) (*nData1)[i + offset1] = corrupted1[i];
		offset1 += corrupted1.size();
		for(i = 0; i < corrupted2.size(); ++i) (*nData2)[i + offset2] = corrupted2[i];
		offset2 += corrupted2.size();


		realMask.read();
		ViSampleChunk &realMask1 = realMask.splitSamples(0);
		ViSampleChunk &realMask2 = realMask.splitSamples(1);

		for(i = 0; i < realMask1.size(); ++i) (*nRealMask1)[i + offset3] = realMask1[i];
		offset3 += realMask1.size();
		for(i = 0; i < realMask2.size(); ++i) (*nRealMask2)[i + offset4] = realMask2[i];
		offset4 += realMask2.size();
	}

	mCurrentObject->clearBuffer(ViAudio::Target);
	mCurrentObject->clearBuffer(ViAudio::Noise);
	mCurrentObject->clearBuffer(ViAudio::NoiseMask);

	ViNoise noise1(nData1, nMask1, mCurrentThreshold);
	ViNoise noise2(nData2, nMask2, mCurrentThreshold);

	for(mCurrentThreshold = MASK_START; mCurrentThreshold <= MASK_END; mCurrentThreshold += MASK_INTERVAL)
	{
		noise1.setThreshold(mCurrentThreshold);
		noise1.generateMask(ViNoise::NOISE_TYPE);
		noise2.setThreshold(mCurrentThreshold);
		noise2.generateMask(ViNoise::NOISE_TYPE);

		qint64 truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0;

		for(i = 0; i < nRealMask1->size(); ++i)
		{
			if((*nRealMask1)[i] == 1)
			{
				if((*nMask1)[i] == 1) ++truePositives;
				else ++falseNegatives;
			}
			else if((*nRealMask1)[i] == 0)
			{
				if((*nMask1)[i] == 1) ++falsePositives;
				else ++trueNegatives;
			}
		}

		for(i = 0; i < nRealMask2->size(); ++i)
		{
			if((*nRealMask2)[i] == 1)
			{
				if((*nMask2)[i] == 1) ++truePositives;
				else ++falseNegatives;
			}
			else if((*nRealMask2)[i] == 0)
			{
				if((*nMask2)[i] == 1) ++falsePositives;
				else ++trueNegatives;
			}
		}

		qreal math = qSqrt(truePositives + falsePositives) * qSqrt(truePositives + falseNegatives) * qSqrt(trueNegatives + falsePositives) * qSqrt(trueNegatives + falseNegatives);
		if(math != 0) math = ((truePositives * trueNegatives) - (falsePositives * falseNegatives)) / math;

		if(math > maxMAT)
		{
			maxMAT = math;
			maxTP = truePositives;
			maxTN = trueNegatives;
			maxFP = falsePositives;
			maxFN = falseNegatives;
			noChange = 0;
		}
		++noChange;
		if(noChange > NO_CHANGE) break;
	}

	delete nRealMask1;
	delete nRealMask2;

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << (int)mParamsCurrent[i] << "\t";
	mOutputStream << (mCurrentThreshold / mDetector->amplification()) << "\t" << mCurrentThreshold << "\t" << maxTP << "\t" << maxTN << "\t" << maxFP << "\t" << maxFN << "\t";
	if((maxTP + maxFN) == 0) mOutputStream << 0 << "\t";
	else mOutputStream << maxTP / qreal(maxTP + maxFN) << "\t";
	if((maxTN + maxFP) == 0) mOutputStream << 0 << "\t";
	else mOutputStream << maxTN / qreal(maxTN + maxFP) << "\t";
	mOutputStream << maxMAT << "\t" << time << "\n";
	mOutputStream.flush();

	++mDoneParamIterations;
	qreal percentageDone = mDoneParamIterations / double(mTotalParamIterations);
	percentageDone /= mTotalFiles;
	percentageDone += (mTotalFiles - mFiles.size() - 1) / qreal(mTotalFiles);

	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0/percentageDone) * remaining) - remaining;
	cout << int(percentageDone * 100.0) << "% ("<<timeConversion(remaining).toLatin1().data()<<" remaining)"<<endl;

	for(i = 0; i < mParamsStart.size(); ++i) cout << (int)mParamsCurrent[i] << "\t";
	cout << (mCurrentThreshold / mDetector->amplification())<< "\t" << mCurrentThreshold << "\t\t" << maxTP << "\t" << maxTN << "\t" << maxFP << "\t" << maxFN << "\t";
	cout << setprecision(10) << maxTP / qreal(maxTP + maxFN) << "\t";
	cout << setprecision(10) << maxTN / qreal(maxTN + maxFP) << "\t";
	cout << maxMAT << "\t" << time << endl;

	if(nextParam()) process1(false);
	else nextFile();
}

QString ViBenchMarker3::timeConversion(int msecs)
{
	QString formattedTime;

	int days = msecs/(1000*60*60*24);
	int hours = (msecs-(days*1000*60*60*24))/(1000*60*60);
	int minutes = (msecs-(days*1000*60*60*24)-(hours*1000*60*60))/(1000*60);
	int seconds = (msecs-(days*1000*60*60*24)-(minutes*1000*60)-(hours*1000*60*60))/1000;

	formattedTime.append(QString("%1").arg(days, 2, 10, QLatin1Char('0')) + ":" +
						 QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

	return formattedTime;
}


void ViBenchMarker3::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}
