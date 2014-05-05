#include <vibenchmarker4.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <QTextStream>
#include <iomanip>

#include <vicosineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <viarmainterpolator.h>

#define AI_NOISE_SEPERATION 512
#define AI_NOISE_START_LENGTH 1
#define AI_NOISE_END_LENGTH 30

#define WINDOW_SIZE 4096
#define MASK_START 0
#define MASK_END 1
//#define MASK_INTERVAL 0.0001
#define MASK_INTERVAL 0.001
#define NOISE_TYPE Direct

#define NO_CHANGE 50

ViBenchMarker4::ViBenchMarker4()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	//mInterpolator = ViCosineInterpolator();
}

ViBenchMarker4::~ViBenchMarker4()
{
}

void ViBenchMarker4::initParams()
{
	mParamsStart.clear();
	mParamsEnd.clear();
	mParamsIncrease.clear();
	mParamsCurrent.clear();

	mParamsStart.append(1);
	mParamsEnd.append(1);
	mParamsIncrease.append(1);
	mParamsCurrent.append(0);

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

		/*mOutputStream<<mInterpolator->name()<<"\n\n";
		mOutputStream << QFileInfo(mCurrentFile).fileName();
		mOutputStream<<"\n\n";*/

		for(int i = 0; i < mParamsStart.size(); ++i)
		{
			mOutputStream << "PARAMETER "<<(i+1) << "\t";
		}
		mOutputStream << "ACCURACY" << "\t" << "TIME" << "\n";

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();

		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker4::process1(bool generate)
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
	if(generate) generateNoise();
	//mCurrentObject->clearBuffer(ViAudio::Target);
	//mCurrentObject->clearBuffer(ViAudio::Noise);
	//mCurrentObject->clearBuffer(ViAudio::NoiseMask);

	/*if(mParamsStart.size() == 1) mDetector->setParameters(mParamsCurrent[0]);
	else if(mParamsStart.size() == 2) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1]);
	else if(mParamsStart.size() == 3) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2]);
	else if(mParamsStart.size() == 4) mDetector->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2], mParamsCurrent[3]);
	else { cout << "Invalid parameter count of "<<mParamsStart.size()<<". Min: 1, Max: 4" << endl; quit(); }*/

	mTime.restart();
	process2();
}

void ViBenchMarker4::process2()
{
	int time = mTime.elapsed();

	mNoChange = 0;

	int halfWindowSize = WINDOW_SIZE / 2;

	ViSampleChunk *mask1 = new ViSampleChunk(halfWindowSize);
	ViSampleChunk *mask2 = new ViSampleChunk(halfWindowSize);
	ViSampleChunk *data1 = new ViSampleChunk(halfWindowSize);
	ViSampleChunk *data2 = new ViSampleChunk(halfWindowSize);
	qint64 offset = 0;
	int i;
	qreal totalAccuracy = 0;
	qint64 accuracyCount = 0;

	//ViInterpolator *interpolator = new ViCosineInterpolator();
	//ViPolynomialInterpolator *interpolator = new ViPolynomialInterpolator(); interpolator->setDegree(1);
	ViInterpolator *interpolator = new ViArmaInterpolator();

	ViAudioReadData read(mCurrentObject->buffer(ViAudio::Target));
	read.setSampleCount(WINDOW_SIZE);
	qint64 done = 0, total = read.bufferSamples();

	write(mNoise1, mNoise2, "/tmp/visore/test.flac");

	while(read.hasData())
	{
		read.read();

		ViSampleChunk &d1 = read.splitSamples(0);
		ViSampleChunk &d2 = read.splitSamples(1);

		if(d1.size() != halfWindowSize || d2.size() != halfWindowSize) break;
		done += d1.size() + d2.size();

		for(i = 0; i < halfWindowSize; ++i)
		{
			(*data1)[i] = d1[i];
			(*data2)[i] = d2[i];
			(*mask1)[i] = mNoise1[offset+i];
			(*mask2)[i] = mNoise2[offset+i];
		}
		offset += halfWindowSize;

		interpolator->interpolate(*data1, *mask1);
		interpolator->interpolate(*data2, *mask2);

		for(i = 0; i < halfWindowSize; ++i)
		{
			if((*mask1)[i])
			{
				totalAccuracy += qAbs((*data1)[i] - d1[i]);
				totalAccuracy += d1[i];
				++accuracyCount;
			}
			if((*mask2)[i])
			{
				totalAccuracy += qAbs((*data2)[i] - d2[i]);
				++accuracyCount;
			}
		}

		qreal percentageDone = (done / qreal(total)) / mTotalFiles;
		qint64 remaining = mMainTime.elapsed();
		remaining = ((1.0/percentageDone) * remaining) - remaining;
		cout << int(percentageDone * 100.0) << "% ("<<timeConversion(remaining).toLatin1().data()<<" remaining)"<<endl;
	}

	cout <<"ACCURACY:" <<"\t"<< (2-(totalAccuracy / accuracyCount))/2.0 << endl;

	delete mask1;
	delete mask2;
	delete data1;
	delete data2;
	delete interpolator;

	if(nextParam())
	{
		process1(false);
	}
	else
	{
		nextFile();
	}
}

void ViBenchMarker4::write(ViSampleChunk &data1, ViSampleChunk &data2, QString path)
{
	mCurrentObject->setFilePath(ViAudio::Corrected, path);
	mCurrentObject->buffer(ViAudio::Corrected)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
	ViAudioWriteData w(mCurrentObject->buffer(ViAudio::Corrected));
	w.setSampleCount(WINDOW_SIZE);
	ViSampleChunk *s = new ViSampleChunk(WINDOW_SIZE);
	qint64 offset = 0;

	for(int i = 0; i < data1.size(); ++i)
	{
		if(offset+WINDOW_SIZE >= data1.size()) break;

		for(int j = 0; j < WINDOW_SIZE; ++j) (*s)[j]= data1[offset+j];
		w.enqueueSplitSamples(*s, 0);

		for(int j = 0; j < WINDOW_SIZE; ++j) (*s)[j]= data2[offset+j];
		w.enqueueSplitSamples(*s, 1);

		offset+=WINDOW_SIZE;
	}

	QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
	mCurrentObject->encode(ViAudio::Corrected);
	while(1) cout<<"p"<<endl;
}

void ViBenchMarker4::generateNoise()
{
	mNoiseLength = AI_NOISE_START_LENGTH;
	mNoiseCount = AI_NOISE_END_LENGTH - mNoiseLength; // Linear add less noise as the size increases

	ViBuffer *buffer = mCurrentObject->buffer(ViAudio::Target);
	ViAudioReadData data(buffer);

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
		//if(c1.size() != 1024)break;
		int index = 0;
		while(index < c1.size())
		{
			index += AI_NOISE_SEPERATION;
			offset1 += AI_NOISE_SEPERATION;
			if(offset1+mNoiseLength <= mNoise1.size())
			{
				for(int i = 0; i < mNoiseLength; ++i) mNoise1[offset1+i] = 1;
				offset1 += AI_NOISE_SEPERATION;
			}

			index += AI_NOISE_SEPERATION;
		}

		ViSampleChunk &c2 = data.splitSamples(1);
		index = 0;
		while(index < c2.size())
		{
			index += AI_NOISE_SEPERATION;
			offset2 += AI_NOISE_SEPERATION;
			if(offset2+mNoiseLength <= mNoise2.size())
			{
				for(int i = 0; i < mNoiseLength; ++i) mNoise2[offset2+i] = 1;
				offset2 += AI_NOISE_SEPERATION;
			}

			index += AI_NOISE_SEPERATION;
		}

		++mNoiseLength;
		if(mNoiseLength > AI_NOISE_END_LENGTH)
		{
			mNoiseLength = AI_NOISE_START_LENGTH;
		}
	}
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
