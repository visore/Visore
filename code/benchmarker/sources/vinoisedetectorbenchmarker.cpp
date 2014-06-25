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

#define WINDOW_SIZE 4096
#define THRESHOLD_NO_CHANGE 50 // Stop searching threshold if no change was observed for n iterations
#define THRESHOLD_DETAIL 1000 // Given the min (0) and max (max value in noise map) threshold, how mnay thresholds should be tested in between

ViNoiseDetectorBenchmarker::ViNoiseDetectorBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	/*mDetector = new ViZscoreNoiseDetector();
	addParam("Window Size", 500, 5120, 5);*/

	/*mDetector = new ViNearestNeighbourNoiseDetector();
	addParam("K", 1, 512, 1);*/

	mDetector = new ViMahalanobisNoiseDetector();
	addParam("Window Size", 30, 5120, 20);

	/*mDetector = new ViMadNoiseDetector();
	addParam("Window Size", 256, 5120, 5);*/

	QObject::connect(mDetector, SIGNAL(progressed(qreal)), this, SLOT(progressDetect(qreal)));
}

ViNoiseDetectorBenchmarker::~ViNoiseDetectorBenchmarker()
{
}

void ViNoiseDetectorBenchmarker::progressDetect(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << (percentage * 0.8) << "%" << flush;
}

void ViNoiseDetectorBenchmarker::progressThreshold(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << (80 + (percentage * 0.2)) << "%" << flush;
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
	int noChange;

	ViClassificationErrorCollection currentErrors, bestErrors;

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::CustomMask), mCurrentObject->buffer(ViAudio::Custom));
	mCurrentObject->clearBuffer(ViAudio::Target);
	mCurrentObject->clearBuffer(ViAudio::CustomMask);

	do
	{
		bestThreshold = 0;
		bestMatthews = -1;
		noChange = 0;
		currentErrors.clear();
		bestErrors.clear();
		mCurrentObject->clearBuffer(ViAudio::Noise);

		for(int i = 0; i < mParamsStart.size(); ++i) mDetector->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mDetector->validParameters())
		{
			mTime.restart();
			qreal maxNoise = mDetector->detect(mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::Noise));
			time = mTime.elapsed();

			// Write
			/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(ViAudio::Noise);
			return;*/

			qreal minThreshold = 0;
			qreal maxThreshold = maxNoise;
			qreal increaseThreshold = roundThreshold(maxNoise / THRESHOLD_DETAIL);

			int total = ((maxThreshold - minThreshold) / increaseThreshold) + 1;
			int current = 0;
			for(qreal t = minThreshold; t <= maxThreshold; t += increaseThreshold)
			{
				currentErrors = mDetector->error(mCurrentObject->buffer(ViAudio::Noise), mCurrentObject->buffer(ViAudio::Custom), t);
				qreal temp = currentErrors.matthewsCoefficient();
				if(temp > bestMatthews)
				{
					noChange = 0;
					bestThreshold = t;
					bestErrors = currentErrors;
					bestMatthews = temp;
				}
				else
				{
					++noChange;
					if(noChange > THRESHOLD_NO_CHANGE) break;
				}
				++current;
				progressThreshold((current * 100.0) / total);
			}
			progressThreshold(100);

			qreal mat = bestErrors.matthewsCoefficient();
			if(mat > maxMatthews) maxMatthews = mat;

			cout << "\r                                                 \r"; // Clear intermidiate percentage
		}
		else
		{
			time = 0;
		}

		// Write
		/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
		mCurrentObject->encode(ViAudio::Corrected);
		return;*/

		++mDoneParamIterations;
		printFileData(bestErrors, time, bestThreshold);
		printTerminal(bestErrors, time, bestThreshold, maxMatthews);
	}
	while(nextParam());

	nextFile();
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

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << "PARAMETER " << i + 1 << " (" << mDetector->parameter(i) <<")\t";
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

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << "PARAMETER " << i + 1 << " (" << mDetector->parameter(i) <<")\t";
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

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << (int) mParamsCurrent[i] << "\t";
	mOutputStream << threshold << "\t" << mDetector->unscaleThreshold(threshold) << "\t" << errors.TP() << "\t" << errors.TN() << "\t" << errors.FP() << "\t" << errors.FN() << "\t" << errors.sensitivity() << "\t" << errors.specificity() << "\t" << errors.matthewsCoefficient() << "\t" << time << "\t\t";
	mOutputStream << errors.at(0).TN() << "\t" << errors.at(0).FP() << "\t" << errors.at(0).specificity() << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream << errors.at(i).TP() << "\t" << errors.at(i).FN() << "\t" << errors.at(i).sensitivity() << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();

	if(time != 0)
	{
		for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << (int) mParamsCurrent[i] << "\t";
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
	cout << "THRES: " << setprecision(6) << threshold << "\tTP: " << errors.TP() << "\tTN: " << errors.TN() << "\tFP: " << errors.FP() << "\tFN: " << errors.FN() << "\tSEN: " << setprecision(6) << errors.sensitivity() << "\tSPE: " << setprecision(6) << errors.specificity() << "\tMAT: " << setprecision(6) << errors.matthewsCoefficient() << " (" << setprecision(6) << maxMatthews << ")\tTIME: " << time << endl;
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
