#include <vinoisebatcher.h>
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
#define THRESHOLD_PARTS 50 // In how many parts a threshold range should be devided in

#define SUMMARY true

ViNoiseBatcher::ViNoiseBatcher()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	/*mDetector = new ViZscoreNoiseDetector();
	addParam("Window Size", 1868, 1868, 1);
	mThreshold = 0.109645025;*/

	/*mDetector = new ViNearestNeighbourNoiseDetector();
	addParam("K", 324, 324, 1);
	mThreshold = 0.340127125;*/

	/*mDetector = new ViMahalanobisNoiseDetector();
	addParam("Window Size", 3264, 3264, 20);
	mThreshold = 0.32745225;*/

	/*mDetector = new ViMadNoiseDetector();
	addParam("Window Size", 1600, 1600, 8);
	mThreshold = 0.127406163;*/

	/*mDetector = new ViFourierNoiseDetector();
	addParam("Window Size", 64,64, 128);
	addParam("Range Start", 5, 5, 25);
	addParam("Range End", 25, 25, 25);
	mThreshold = 0.8094575;*/







	/*mDetector = new ViPredictionNoiseDetector(new ViPolynomialPredictor());
	addParam("Window Size", 1560, 1560, 1);
	addParam("Degree", 1, 1, 1);
	mThreshold = 0.189501;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViPolynomialPredictor(ViPolynomialPredictor::Osculating));
	addParam("Window Size", 48, 48, 1);
	addParam("Degree", 2, 2, 1);
	addParam("Derivatives", 1, 1, 1);
	mThreshold = 0.196139625;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViFourierPredictor());
	addParam("Window Size", 64, 64, 1);
	addParam("Degree", 10, 10, 1);
	mThreshold = 0.22009925;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViFourierPredictor(ViFourierPredictor::Osculating));
	addParam("Window Size", 64, 64, 1);
	addParam("Degree", 10, 10, 1);
	addParam("Derivatives", 1, 1, 8);
	mThreshold = 0.21217875;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViHermitePredictor());
	addParam("Window Size", 2, 2, 1);
	mThreshold = 0.195884;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViNewtonPredictor());
	addParam("Window Size", 2, 2, 1);
	mThreshold = 0.19807075;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViLagrangePredictor());
	addParam("Window Size", 2, 2, 1);
	mThreshold = 0.192705;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViArmaPredictor());
	addParam("Window Size", 64, 64, 1);
	addParam("AR Degree", 2, 2, 1);
	addParam("I Degree", 0, 0, 1);
	addParam("MA Degree", 0, 0, 1);
	mThreshold = 0.204728;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViArmaPredictor());
	addParam("Window Size", 508, 508, 1);
	addParam("AR Degree", 0, 0, 1);
	addParam("I Degree", 0, 0, 1);
	addParam("MA Degree", 1, 1, 1);
	mThreshold = 0.194168375;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViArmaPredictor());
	addParam("Window Size", 64, 64, 1);
	addParam("AR Degree", 2, 2, 1);
	addParam("I Degree", 0, 0, 1);
	addParam("MA Degree", 2, 2, 1);
	mThreshold = 0.204854125;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViArmaPredictor());
	addParam("Window Size", 192, 192, 1);
	addParam("AR Degree", 8, 8, 1);
	addParam("I Degree", 1, 1, 1);
	addParam("MA Degree", 2, 2, 1);
	mThreshold = 0.196836;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViGarchPredictor());
	addParam("Window Size", 48, 48, 1);
	addParam("Arch Degree", 1, 1, 1);
	addParam("Garch Degree", 0, 0, 1);
	mThreshold = 0.197459625;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViGarchPredictor());
	addParam("Window Size", 48, 48, 1);
	addParam("Arch Degree", 1, 1, 1);
	addParam("Garch Degree", 1, 1, 1);
	mThreshold = 0.197459625;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViNeuralPredictor(ViNeuralPredictor::IncrementalSet));
	addParam("Window Size", 4, 4, 1);
	addParam("Hidden Layer 1", 8, 8, 1);
	mThreshold = 0.21376275;*/

	/*mDetector = new ViPredictionNoiseDetector(new ViNeuralPredictor(ViNeuralPredictor::IncrementalRecurrent));
	addParam("Window Size", 12, 12, 1);
	mThreshold = 0.211363;*/


	mDetector->setMode(ViNoiseDetector::Standard);

	QObject::connect(mDetector, SIGNAL(progressed(qreal)), this, SLOT(progressDetect(qreal)));
}

ViNoiseBatcher::~ViNoiseBatcher()
{
}

void ViNoiseBatcher::progressDetect(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << (percentage * 0.9) << "% " << flush;
}

void ViNoiseBatcher::progressThreshold()
{
	cout << "." << flush;
}

void ViNoiseBatcher::clearProgress()
{
	cout << "\r                                                                                                                                 \r"; // Clear intermidiate percentage
}

void ViNoiseBatcher::addParam(QString name, qreal start, qreal end, qreal increase)
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

void ViNoiseBatcher::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViNoiseBatcher::nextParam()
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

void ViNoiseBatcher::benchmark(QString folder)
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

void ViNoiseBatcher::addDir(QString dirName)
{
	QDir dirFile("/home/visore/Visore Projects/Files/" + dirName);
	if(!dirFile.exists())
	{
		cout << "Directory does not exist: " << dirFile.absolutePath().toLatin1().data() << endl;
		return;
	}

	QDir dirResult("/home/visore/Visore Projects/Results/" + dirName);
	if(!dirResult.exists()) dirResult.mkpath(dirResult.absolutePath());

	if(SUMMARY)
	{
		QString summary = dirResult.absolutePath() + "/summary.txt";
		if(!mSummaryFiles.contains(summary))
		{
			printFileHeader(summary);
			mSummaryFiles.append(summary);
		}
	}

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

void ViNoiseBatcher::nextFile()
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

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		mCurrentObject->mDisbaleFinishSignal = true; // The finish signal sometimes gives a seg fault
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));
		mCurrentObject->decode();
	}
}

void ViNoiseBatcher::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));

	qint64 time;

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::CustomMask), mCurrentObject->buffer(ViAudio::Custom));
	mCurrentObject->clearBuffer(ViAudio::Target);
	mCurrentObject->clearBuffer(ViAudio::CustomMask);

	ViClassificationErrorCollection errors;

	//do
	//{
		mCurrentObject->clearBuffer(ViAudio::Noise);

		for(int i = 0; i < mParamsStart.size(); ++i) mDetector->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mDetector->validParameters())
		{
			mTime.restart();
			mDetector->detect(mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::Noise));
			time = mTime.elapsed();

			errors = mDetector->error(mCurrentObject->buffer(ViAudio::Noise), mCurrentObject->buffer(ViAudio::Custom), mThreshold);

			// Write
		/*	QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(ViAudio::Noise);
			return;*/

			/*calculateThreshold(mCurrentObject->buffer(ViAudio::Noise), mCurrentObject->buffer(ViAudio::Custom), bestThreshold, bestErrors, bestMatthews, maxNoise);
			if(bestMatthews > maxMatthews) maxMatthews = bestMatthews;
			clearProgress();*/

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
		printFileData(errors, time);
		printTerminal(errors, time);
	//}
	//while(nextParam());

	nextFile();
}

void ViNoiseBatcher::printFileHeader(QString filepath)
{
	int i;

	mOutputFile.close();
	mOutputFile.setFileName(filepath);
	mOutputFile.open(QIODevice::WriteOnly);
	QTextStream stream(&mOutputFile);

	stream << mDetector->name() << "\n\n";
	stream << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) stream << "PARAMETER " << i + 1 << " (" << mParamsNames[i] <<")\t";
	stream << "THRESHOLD (SCALED)" << "\t" << "THRESHOLD (REAL)" << "\t" << "TP" << "\t" << "TN" << "\t" << "FP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t" << "SPECIFICITY" << "\t" << "MATTHEWS COEFFICIENT" << "\t" << "TIME" << "\t\t";
	stream << "NO NOISE " << "\t\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << "NOISE SIZE " << i << "\t\t\t";
	stream << "\n";
	for(i = 0; i < mParamsStart.size(); ++i) stream << "\t";
	stream << "\t\t\t\t\t\t\t\t\t\t\t";
	stream << "TN" << "\t" << "FP" << "\t" << "SPECIFICITY" << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << "TP" << "\t" << "FN" << "\t" << "SENSITIVITY" << "\t";
	stream << "\n";
	stream.flush();
}

void ViNoiseBatcher::printFileData(ViClassificationErrorCollection &errors, const qint64 &time)
{
	int i;
	QString filepath = "";
	for(i = 0; i < mSummaryFiles.size(); ++i)
	{
		if(mSummaryFiles[i].contains(genre(mSummaryFiles[i])))
		{
			filepath = mSummaryFiles[i];
			break;
		}
	}
	if(filepath == "") return;

	QFile file(filepath);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);



	for(i = 0; i < mParamsStart.size(); ++i)
	{
		if((int) mParamsCurrent[i] == mParamsCurrent[i]) stream << (int) mParamsCurrent[i] << "\t";
		else stream << mParamsCurrent[i] << "\t";
	}
	stream << errors.TP() << "\t" << errors.TN() << "\t" << errors.FP() << "\t" << errors.FN() << "\t" << errors.sensitivity() << "\t" << errors.specificity() << "\t" << errors.matthewsCoefficient() << "\t" << time << "\t\t";
	stream << errors.at(0).TN() << "\t" << errors.at(0).FP() << "\t" << errors.at(0).specificity() << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << errors.at(i).TP() << "\t" << errors.at(i).FN() << "\t" << errors.at(i).sensitivity() << "\t";
	stream << "\n";
	stream.flush();
}

void ViNoiseBatcher::printTerminal(ViClassificationErrorCollection &errors, const qint64 &time)
{
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "\tSEN: " << setprecision(6) << errors.sensitivity() << "\tSPE: " << setprecision(6) << errors.specificity() << "\tMAT: " << setprecision(6) << errors.matthewsCoefficient() << "\tTIME: " << time << endl;
}

qreal ViNoiseBatcher::roundThreshold(const qreal &value)
{
	static const int precision = 5;
	static const int adjustment = pow(10, precision);

	return viround((value * adjustment) + 0.5) / qreal(adjustment);
}

QString ViNoiseBatcher::timeConversion(int msecs)
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

void ViNoiseBatcher::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}

QString ViNoiseBatcher::genre(QString path)
{
	path = path.replace("/home/visore/Visore Projects/Files/", "");
	int i = path.indexOf("/");
	if(i < 0) return "";
	return path.left(i).replace("/", "");
}
