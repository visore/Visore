#include <vibenchmarker4.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <vilogger.h>
#include <QTextStream>
#include <iomanip>

#include <vicosineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <visplineinterpolator.h>
#include <vihermiteinterpolator.h>
#include <vifourierinterpolator.h>
#include <viarmainterpolator.h>

#define WINDOW_SIZE 4096
#define MASK_START 0
#define MASK_END 1
//#define MASK_INTERVAL 0.0001
#define MASK_INTERVAL 0.001
#define NOISE_TYPE Direct

//#define WRITE_CORRECTED

#define NO_CHANGE 50

ViBenchMarker4::ViBenchMarker4()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	//mInterpolator = new ViCosineInterpolator();
	mInterpolator = new ViPolynomialInterpolator();
	//mInterpolator = new ViSplineInterpolator();
	//mInterpolator = new ViArmaInterpolator();
	//mInterpolator = new ViHermiteInterpolator();
	//mInterpolator = new ViFourierInterpolator();

	/*addParam("WINDOW SIZE", 5, 5, 1);
	addParam("ARDEGREE", 5, 5, 1);
	addParam("MADEGREE", 0, 0, 1);*/

	addParam("WINDOW SIZE", 1, 20, 1);
	addParam("DEGREE", 1,10, 1);
}

ViBenchMarker4::~ViBenchMarker4()
{
}

void ViBenchMarker4::addParam(QString name, qreal start, qreal end, qreal increase)
{
	mParamsName.append(name);
	mParamsStart.append(start);
	mParamsEnd.append(end);
	mParamsIncrease.append(increase);
	mParamsCurrent.append(0);
}

void ViBenchMarker4::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViBenchMarker4::nextParam()
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

void ViBenchMarker4::benchmark()
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

void ViBenchMarker4::nextFile()
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

		mOutputStream<<mInterpolator->name()<<"\n\n";
		mOutputStream << QFileInfo(mCurrentFile).fileName();
		mOutputStream<<"\n\n";

		for(int i = 0; i < mParamsName.size(); ++i)
		{
			mOutputStream << "PARAMETER "<<(i+1) << " (" << mParamsName[i] << ")\t";
		}
		mOutputStream << "ACCURACY" << "\t" << "TIME" << "\n";

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();

		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker4::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::NoiseMask));

	int i, halfWindowSize = WINDOW_SIZE / 2;
	qint64 offset, accuracyCount, time;
	qreal totalAccuracy, acc, percentageDone, remaining, best = 0;

	ViSampleChunk data1(halfWindowSize);
	ViSampleChunk data2(halfWindowSize);

	do
	{

		if(mParamsStart.size() == 1) mInterpolator->setParameters(mParamsCurrent[0]);
		else if(mParamsStart.size() == 2) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1]);
		else if(mParamsStart.size() == 3) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2]);
		else if(mParamsStart.size() == 4) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2], mParamsCurrent[3]);
		else { cout << "Invalid parameter count of "<<mParamsStart.size()<<". Min: 1, Max: 4" << endl; quit(); }

		ViAudioReadData target(mCurrentObject->buffer(ViAudio::Target));
		ViAudioReadData corrupted(mCurrentObject->buffer(ViAudio::Corrupted));
		ViAudioReadData noise(mCurrentObject->buffer(ViAudio::NoiseMask));
		target.setSampleCount(WINDOW_SIZE);
		corrupted.setSampleCount(WINDOW_SIZE);
		noise.setSampleCount(WINDOW_SIZE);

		offset = 0;
		totalAccuracy = 0;
		accuracyCount = 0;

		// Write
		#ifdef WRITE_CORRECTED
			ViAudio::Type outputType = ViAudio::NoiseMask;
			mCurrentObject->buffer(outputType)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
			ViAudioWriteData write(mCurrentObject->buffer(outputType));
			write.setSampleCount(WINDOW_SIZE);
		#endif

		mTime.restart();
		while(corrupted.hasData())
		{	mParamsStart.clear();
			mParamsEnd.clear();
			mParamsIncrease.clear();
			mParamsCurrent.clear();



			target.read();
			ViSampleChunk &t1 = target.splitSamples(0);
			ViSampleChunk &t2 = target.splitSamples(1);

			corrupted.read();
			ViSampleChunk &c1 = corrupted.splitSamples(0);
			ViSampleChunk &c2 = corrupted.splitSamples(1);

			noise.read();
			ViSampleChunk &n1 = noise.splitSamples(0);
			ViSampleChunk &n2 = noise.splitSamples(1);

			if(c1.size() != halfWindowSize || c2.size() != halfWindowSize) break;

			for(i = 0; i < halfWindowSize; ++i)
			{
				data1[i] = c1[i];
				data2[i] = c2[i];
			}
			offset += halfWindowSize;

			mInterpolator->interpolate(data1, n1);
			mInterpolator->interpolate(data2, n2);

			// Write
			#ifdef WRITE_CORRECTED
				if(outputType == ViAudio::Corrected)
				{
					write.enqueueSplitSamples(data1, 0);
					write.enqueueSplitSamples(data2, 1);
				}
				else if(outputType == ViAudio::Corrupted)
				{
					write.enqueueSplitSamples(c1, 0);
					write.enqueueSplitSamples(c2, 1);
				}
				else if(outputType == ViAudio::NoiseMask)
				{
					write.enqueueSplitSamples(n1, 0);
					write.enqueueSplitSamples(n2, 1);
				}
			#endif

			for(i = 0; i < halfWindowSize; ++i)
			{
				if(n1[i])
				{
					totalAccuracy += qAbs(data1[i] - t1[i]);
					++accuracyCount;
				}
				if(n2[i])
				{
					totalAccuracy += qAbs(data2[i] - t2[i]);
					++accuracyCount;
				}
			}
		}
		time = mTime.elapsed();

		++mDoneParamIterations;
		acc = (2 - (totalAccuracy / accuracyCount)) / 2.0;
		if(acc > best) best = acc;
		percentageDone = mDoneParamIterations / double(mTotalParamIterations);
		remaining = mMainTime.elapsed();

		remaining = ((1.0/percentageDone) * remaining) - remaining;
		cout << int(percentageDone * 100.0) << "%\t("<<timeConversion(remaining).toLatin1().data()<<")\tAccuracy: "<<acc<<" ("<<best<<")\tTime: "<<time<<endl;

		for(i = 0; i < mParamsStart.size(); ++i)
		{
			mOutputStream << (int) mParamsCurrent[i] << "\t";
		}
		mOutputStream << acc << "\t" << time << "\n";
		mOutputStream.flush();

		// Write
		#ifdef WRITE_CORRECTED
			QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(outputType);
			return;
		#endif

	}
	while(nextParam());

	nextFile();
}

QString ViBenchMarker4::timeConversion(int msecs)
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


void ViBenchMarker4::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}
