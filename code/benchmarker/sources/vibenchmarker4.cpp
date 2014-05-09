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

	//mInterpolator = new ViCosineInterpolator();
	mInterpolator = new ViPolynomialInterpolator();
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
	mParamsStart.clear();
	mParamsEnd.clear();
	mParamsIncrease.clear();
	mParamsCurrent.clear();

	addParam("WINDOW SIZE", 100, 200, 1);
	addParam("DEGREE", 1, 20, 1);

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
		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		//quit();
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
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker4::process1(bool generate)
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
	//if(generate) generateNoise();

	//mCurrentObject->clearBuffer(ViAudio::Target);
	//mCurrentObject->clearBuffer(ViAudio::Noise);
	//mCurrentObject->clearBuffer(ViAudio::NoiseMask);

	if(mParamsStart.size() == 1) mInterpolator->setParameters(mParamsCurrent[0]);
	else if(mParamsStart.size() == 2) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1]);
	else if(mParamsStart.size() == 3) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2]);
	else if(mParamsStart.size() == 4) mInterpolator->setParameters(mParamsCurrent[0], mParamsCurrent[1], mParamsCurrent[2], mParamsCurrent[3]);
	else { cout << "Invalid parameter count of "<<mParamsStart.size()<<". Min: 1, Max: 4" << endl; quit(); }


	process2();
}

void ViBenchMarker4::process2()
{
	mTime.restart();
	mNoChange = 0;

	int halfWindowSize = WINDOW_SIZE / 2;
	int i;
	qint64 offset = 0;
	qreal totalAccuracy = 0;
	qint64 accuracyCount = 0;

	//ViAudioReadData target(mCurrentObject->buffer(ViAudio::Target));
	/*ViAudioReadData corrupted(mCurrentObject->buffer(ViAudio::Corrupted));
	target.setSampleCount(WINDOW_SIZE);
	corrupted.setSampleCount(WINDOW_SIZE);

	ViSampleChunk mask1(halfWindowSize);
	ViSampleChunk mask2(halfWindowSize);
	ViSampleChunk data1(halfWindowSize);
	ViSampleChunk data2(halfWindowSize);

	//write(mNoise1, mNoise2, "/tmp/visore/test.flac"); return;

	// Write
		//mCurrentObject->buffer(ViAudio::Corrected)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
		//ViAudioWriteData write(mCurrentObject->buffer(ViAudio::Corrected));
		//write.setSampleCount(WINDOW_SIZE);

	while(corrupted.hasData())
	{
		target.read();
		ViSampleChunk &t1 = target.splitSamples(0);
		ViSampleChunk &t2 = target.splitSamples(1);

		corrupted.read();
		ViSampleChunk &c1 = corrupted.splitSamples(0);
		ViSampleChunk &c2 = corrupted.splitSamples(1);

		if(c1.size() != halfWindowSize || c2.size() != halfWindowSize) break;

		for(i = 0; i < halfWindowSize; ++i)
		{
			data1[i] = c1[i];
			data2[i] = c2[i];
			mask1[i] = mNoise1[offset+i];
			mask2[i] = mNoise2[offset+i];
		}
		offset += halfWindowSize;

		mInterpolator->interpolate(c1, mask1);
		mInterpolator->interpolate(c2, mask2);

		// Write
			//write.enqueueSplitSamples(*data1, 0);
			//write.enqueueSplitSamples(*data2, 1);

		for(i = 0; i < halfWindowSize; ++i)
		{
			if(mask1[i])
			{
				totalAccuracy += qAbs(data1[i] - t1[i]);
				++accuracyCount;
			}
			if(mask2[i])
			{
				totalAccuracy += qAbs(data2[i] - c2[i]);
				++accuracyCount;
			}
		}
	}*/

	++mDoneParamIterations;

	qint64 time = mTime.elapsed();
	qreal acc = (2 - (totalAccuracy / accuracyCount)) / 2.0;
	qreal percentageDone = mDoneParamIterations / double(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();

	remaining = ((1.0/percentageDone) * remaining) - remaining;
	cout << int(percentageDone * 100.0) << "%\t("<<timeConversion(remaining).toLatin1().data()<<")\tAccuracy: "<<acc<<"\tTime: "<<time<<endl;

	for(i = 0; i < mParamsStart.size(); ++i)
	{
		mOutputStream << (int) mParamsCurrent[i] << "\t";
	}
	mOutputStream << acc << "\t" << time << "\n";
	mOutputStream.flush();

	// Write
		//QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
		//mCurrentObject->encode(ViAudio::Corrected);
		//return;

	if(nextParam()) process1(false);
	else nextFile();
}

void ViBenchMarker4::write(ViSampleChunk &data1, ViSampleChunk &data2, QString path)
{
	mCurrentObject->setFilePath(ViAudio::Corrected, path);
	mCurrentObject->buffer(ViAudio::Corrected)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
	ViAudioWriteData w(mCurrentObject->buffer(ViAudio::Corrected));
	w.setSampleCount(WINDOW_SIZE);

	int halfWindowSize = WINDOW_SIZE / 2;
	ViSampleChunk s(halfWindowSize);
	qint64 offset = 0;

	for(int i = 0; i < data1.size(); ++i)
	{
		if(offset+halfWindowSize >= data1.size()) break;

		for(int j = 0; j < halfWindowSize; ++j) s[j]= data1[offset+j];
		w.enqueueSplitSamples(s, 0);

		for(int j = 0; j < halfWindowSize; ++j) s[j]= data2[offset+j];
		w.enqueueSplitSamples(s, 1);

		offset+=halfWindowSize;
	}

	QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
	mCurrentObject->encode(ViAudio::Corrected);
}

void ViBenchMarker4::generateNoise()
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
		if(c1.size() != 1024)break;
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
				for(int i = 0; i < length; ++i) mNoise2[offset2+i] = 1;
				offset2 += AI_NOISE_SEPERATION;
			}

			index += AI_NOISE_SEPERATION;
		}
		data2.enqueueSplitSamples(c2, 1);
	}
}

int ViBenchMarker4::addNoise(ViSampleChunk &s, int offset)
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

void ViBenchMarker4::addNoise1(ViSampleChunk &s, int offset, int length)
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

void ViBenchMarker4::addNoise2(ViSampleChunk &s, int offset, int length)
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

void ViBenchMarker4::addNoise3(ViSampleChunk &s, int offset, int length)
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
