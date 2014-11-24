#include <vinoisedetectorbenchmarker.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

#include <vizscorenoisedetector.h>
#include <vinearestneighbournoisedetector.h>
#include <vimahalanobisnoisedetector.h>
#include <vimadnoisedetector.h>
#include <vifouriernoisedetector.h>
#include <vipredictionnoisedetector.h>

#include <viconstantpredictor.h>
#include <vipolynomialpredictor.h>
#include <vihermitepredictor.h>
#include <vifourierpredictor.h>
#include <viarmapredictor.h>
#include <vigarchpredictor.h>
#include <vilagrangepredictor.h>
#include <vinewtonpredictor.h>
#include <vineuralpredictor.h>

#define WINDOW_SIZE 4096
#define THRESHOLD_PARTS 20 // In how many parts a threshold range should be devided in

ViNoiseDetectorBenchmarker::ViNoiseDetectorBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	mDetector = new ViZscoreNoiseDetector();
	addParam("Window Size", 1024, 4096, 64);

	/*mDetector = new ViNearestNeighbourNoiseDetector();
	addParam("K", 1, 512, 1);*/

	/*mDetector = new ViMahalanobisNoiseDetector();
	addParam("Window Size", 30, 5120, 20);*/

	/*mDetector = new ViMadNoiseDetector();
	addParam("Window Size", 256, 5120, 5);*/

	/*mDetector = new ViFourierNoiseDetector();
	addParam("Window Size", 4096,4096, 128);
	addParam("Range Start", 0, 0, 25);
	addParam("Range End", 100, 100, 25);*/

	/*mDetector = new ViPredictionNoiseDetector(new ViArmaPredictor());
	addParam("Window Size", 256, 256, 8);
	addParam("AR Degree", 2, 2, 1);
	addParam("MA Degree", 1, 1, 1);
	addParam("Threshold", 0.55, 0.75, 0.005);*/

	/*mDetector = new ViPredictionNoiseDetector(new ViNeuralPredictor());
	addParam("Window Size", 1, 1, 8);*/

	QObject::connect(mDetector, SIGNAL(progressed(qreal)), this, SLOT(progressDetect(qreal)));
}

ViNoiseDetectorBenchmarker::~ViNoiseDetectorBenchmarker()
{
}

void ViNoiseDetectorBenchmarker::progressDetect(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << (percentage * 0.9) << "% " << flush;
}

void ViNoiseDetectorBenchmarker::progressThreshold()
{
	cout << "." << flush;
}

void ViNoiseDetectorBenchmarker::clearProgress()
{
	cout << "\r                                                                                                                                 \r"; // Clear intermidiate percentage
}

void ViNoiseDetectorBenchmarker::addParam(QString name, qreal start, qreal end, qreal increase)
{
	if(mDetector->hasParameter(name))
	{
		mParamsNames.append(name);
		mParamsStart.append(start);
		mParamsEnd.append(end);
		mParamsIncrease.append(increase);
		mParamsCurrent.append(0);
	}
	else cout << "This noise detector (" << mDetector->name("", true).toLatin1().data() << ") doesn't make use of the given parameter (" << name.toLatin1().data() << ")." << endl;
}

void ViNoiseDetectorBenchmarker::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViNoiseDetectorBenchmarker::nextParam()
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

void ViNoiseDetectorBenchmarker::benchmark(QString folder)
{
	if(folder == "")
	{
		QDir dir("/home/visore/Visore Projects/Files/");
		QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		for(int i = 0; i < dirs.size(); ++i) addDir(dirs[i]);
	}
	else
	{
		addDir(folder);
	}

	mTotalFiles = mFiles.size();
	nextFile();
}

void ViNoiseDetectorBenchmarker::addDir(QString dirName)
{
	QDir dirFile("/home/visore/Visore Projects/Files/" + dirName);
	if(!dirFile.exists())
	{
		cout << "Directory does not exist: " << dirFile.absolutePath().toLatin1().data() << endl;
		return;
	}

	QDir dirResult("/home/visore/Visore Projects/Results/" + dirName);
	if(!dirResult.exists()) dirResult.mkpath(dirResult.absolutePath());

	QDir dir(dirFile.absolutePath());
	QStringList files = dir.entryList(QDir::Files);
	for(int j = 0; j < files.size(); ++j)
	{
		mFiles.enqueue(dir.absoluteFilePath(files[j]));
		QString id = ViId::generate();
		mResults.enqueue(dirResult.absolutePath() + "/" + mDetector->name() + "_" + id + "_ALL.txt");
		mResults2.enqueue(dirResult.absolutePath() + "/" + mDetector->name() + "_" + id + "_MINIFIED.txt");
	}
}

void ViNoiseDetectorBenchmarker::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
	{
		initParams();
		for(int i = 0; i < mParamsStart.size(); ++i) mParamsCurrent[i] = mParamsStart[i];
		mCurrentFile = mFiles.dequeue();
		printFileHeader();

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));
		mCurrentObject->decode();
	}
}

void ViNoiseDetectorBenchmarker::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));

	qreal maxMatthews = -1;
	qint64 time;
	qreal bestThreshold, bestMatthews;

	ViClassificationErrorCollection bestErrors;

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::CustomMask), mCurrentObject->buffer(ViAudio::Custom));
	mCurrentObject->clearBuffer(ViAudio::Target);
	mCurrentObject->clearBuffer(ViAudio::CustomMask);

	do
	{
		bestThreshold = 0;
		bestMatthews = -1;
		bestErrors.clear();

		mCurrentObject->clearBuffer(ViAudio::Noise);

		for(int i = 0; i < mParamsStart.size(); ++i) mDetector->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mDetector->validParameters())
		{
			mTime.restart();
			qreal maxNoise = mDetector->detect(mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::Noise));
			time = mTime.elapsed();

			// Write
		/*	QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(ViAudio::Noise);
			return;*/

			calculateThreshold(mCurrentObject->buffer(ViAudio::Noise), mCurrentObject->buffer(ViAudio::Custom), bestThreshold, bestErrors, bestMatthews, maxNoise);
			if(bestMatthews > maxMatthews) maxMatthews = bestMatthews;
			clearProgress();

			// Write
			/*mDetector->mask(mCurrentObject->buffer(ViAudio::Noise), mCurrentObject->buffer(ViAudio::NoiseMask), 0.15, 8);
			QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(ViAudio::NoiseMask);
			return;*/
		}
		else
		{
			time = 0;
		}

		++mDoneParamIterations;
		printFileData(bestErrors, time, bestThreshold);
		printTerminal(bestErrors, time, bestThreshold, maxMatthews);
	}
	while(nextParam());

	nextFile();
}

void ViNoiseDetectorBenchmarker::calculateThreshold(ViBuffer *noise, ViBuffer *size, qreal &bestThreshold, ViClassificationErrorCollection &bestErrors, qreal &bestMatthews, const qreal &maxNoise)
{
	bestErrors.clear();

	qreal minThreshold = 0;
	qreal maxThreshold = maxNoise;
	qreal part;

	int i, zoomCount = 0;
	bool hasChanged;

	QVector<ViClassificationErrorCollection> errors(THRESHOLD_PARTS + 1, ViClassificationErrorCollection());
	QVector<qreal> thresholds(THRESHOLD_PARTS + 1);

	while(true)
	{
		part = (maxThreshold - minThreshold) / THRESHOLD_PARTS;
		hasChanged = false;

		for(i = 0; i <= THRESHOLD_PARTS; ++i) thresholds[i] = minThreshold + (part * i);
		mDetector->error(noise, size, thresholds, errors);

		for(i = 0; i <= THRESHOLD_PARTS; ++i)
		{
			qreal tempMatthews = errors[i].matthewsCoefficient();
			if(tempMatthews > bestMatthews)
			{
				minThreshold = thresholds[i] - part;
				maxThreshold = thresholds[i] + part;
				if(minThreshold < 0) minThreshold = 0;
				if(maxThreshold > maxNoise) maxThreshold = maxNoise;

				bestThreshold = thresholds[i];
				bestErrors = errors[i];
				bestMatthews = tempMatthews;

				hasChanged = true;
			}
		}
		if(!hasChanged) break;
		++zoomCount;
		progressThreshold();
	}
}

void ViNoiseDetectorBenchmarker::printFileHeader()
{	
	int i;

	mOutputFile.close();
	mOutputFile.setFileName(mResults.dequeue());
	mOutputFile.open(QIODevice::WriteOnly);
	mOutputStream.setDevice(&mOutputFile);

	mOutputStream << mDetector->name() << "\n\n";
	mOutputStream << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << "PARAMETER " << i + 1 << " (" << mParamsNames[i] <<")\t";
	mOutputStream << "THRESHOLD (SCALED)" << "\t" << "THRESHOLD (REAL)" << "\t" << "TP" << "\t" << "TN" << "\t" << "FP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t" << "SPECIFICITY" << "\t" << "MATTHEWS COEFFICIENT" << "\t" << "TIME" << "\t\t";
	mOutputStream << "NO NOISE " << "\t\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream << "NOISE SIZE " << i << "\t\t\t";
	mOutputStream << "\n";
	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << "\t";
	mOutputStream << "\t\t\t\t\t\t\t\t\t\t\t";
	mOutputStream << "TN" << "\t" << "FP" << "\t" << "SPECIFICITY" << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream << "TP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();

	mOutputFile2.close();
	mOutputFile2.setFileName(mResults2.dequeue());
	mOutputFile2.open(QIODevice::WriteOnly);
	mOutputStream2.setDevice(&mOutputFile2);

	mOutputStream2 << mDetector->name() << "\n\n";
	mOutputStream2 << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << "PARAMETER " << i + 1 << " (" << mParamsNames[i] <<")\t";
	mOutputStream2 << "THRESHOLD (SCALED)" << "\t" << "THRESHOLD (REAL)" << "\t" << "TP" << "\t" << "TN" << "\t" << "FP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t" << "SPECIFICITY" << "\t" << "MATTHEWS COEFFICIENT" << "\t" << "TIME" << "\t\t";
	mOutputStream2 << "NO NOISE " << "\t\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream2 << "NOISE SIZE " << i << "\t\t\t";
	mOutputStream2 << "\n";
	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << "\t";
	mOutputStream2 << "\t\t\t\t\t\t\t\t\t\t\t";
	mOutputStream2 << "TN" << "\t" << "FP" << "\t" << "SPECIFICITY" << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream2 << "TP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t";
	mOutputStream2 << "\n";
	mOutputStream2.flush();
}

void ViNoiseDetectorBenchmarker::printFileData(ViClassificationErrorCollection &errors, const qint64 &time, const qreal &threshold)
{
	int i;

	for(i = 0; i < mParamsStart.size(); ++i)
	{
		if((int) mParamsCurrent[i] == mParamsCurrent[i]) mOutputStream << (int) mParamsCurrent[i] << "\t";
		else mOutputStream << mParamsCurrent[i] << "\t";
	}
	mOutputStream << threshold << "\t" << mDetector->unscaleThreshold(threshold) << "\t" << errors.TP() << "\t" << errors.TN() << "\t" << errors.FP() << "\t" << errors.FN() << "\t" << errors.sensitivity() << "\t" << errors.specificity() << "\t" << errors.matthewsCoefficient() << "\t" << time << "\t\t";
	mOutputStream << errors.at(0).TN() << "\t" << errors.at(0).FP() << "\t" << errors.at(0).specificity() << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream << errors.at(i).TP() << "\t" << errors.at(i).FN() << "\t" << errors.at(i).sensitivity() << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();

	if(time != 0)
	{
		for(i = 0; i < mParamsStart.size(); ++i)
		{
			if((int) mParamsCurrent[i] == mParamsCurrent[i]) mOutputStream2 << (int) mParamsCurrent[i] << "\t";
			else mOutputStream2 << mParamsCurrent[i] << "\t";
		}
		mOutputStream2 << threshold << "\t" << mDetector->unscaleThreshold(threshold) << "\t" << errors.TP() << "\t" << errors.TN() << "\t" << errors.FP() << "\t" << errors.FN() << "\t" << errors.sensitivity() << "\t" << errors.specificity() << "\t" << errors.matthewsCoefficient() << "\t" << time << "\t\t";
		mOutputStream2 << errors.at(0).TN() << "\t" << errors.at(0).FP() << "\t" << errors.at(0).specificity() << "\t";
		for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream2 << errors.at(i).TP() << "\t" << errors.at(i).FN() << "\t" << errors.at(i).sensitivity() << "\t";
		mOutputStream2 << "\n";
		mOutputStream2.flush();
	}
}

void ViNoiseDetectorBenchmarker::printTerminal(ViClassificationErrorCollection &errors, const qint64 &time, const qreal &threshold, const qreal &maxMatthews)
{
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "THRES: " << setprecision(6) << threshold << "\tSEN: " << setprecision(6) << errors.sensitivity() << "\tSPE: " << setprecision(6) << errors.specificity() << "\tMAT: " << setprecision(6) << errors.matthewsCoefficient() << " (" << setprecision(6) << maxMatthews << ")\tTIME: " << time << endl;
}

qreal ViNoiseDetectorBenchmarker::roundThreshold(const qreal &value)
{
	static const int precision = 5;
	static const int adjustment = pow(10, precision);

	return viround((value * adjustment) + 0.5) / qreal(adjustment);
}

QString ViNoiseDetectorBenchmarker::timeConversion(int msecs)
{
	QString formattedTime;

	int days = msecs / 86400000;
	int hours = (msecs - (days * 86400000)) / 3600000;
	int minutes = (msecs - (days * 86400000) - (hours * 3600000)) / 60000;
	int seconds = (msecs - (days * 86400000) - (minutes * 60000) - (hours * 3600000)) / 1000;

	formattedTime.append(QString("%1").arg(days, 2, 10, QLatin1Char('0')) + ":" +
						 QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));

	return formattedTime;
}

void ViNoiseDetectorBenchmarker::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}
