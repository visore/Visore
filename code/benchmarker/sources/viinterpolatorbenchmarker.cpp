#include <viinterpolatorbenchmarker.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

#include <vipolynomialinterpolator.h>
#include <vifourierinterpolator.h>
#include <vilagrangeinterpolator.h>
#include <vinewtoninterpolator.h>
#include <viarimainterpolator.h>
#include <vigarchinterpolator.h>

#define WINDOW_SIZE 4096

ViInterpolatorBenchmarker::ViInterpolatorBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	/*mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Osculating, ViPolynomialInterpolator::Fixed);
	addParam("Window Size", 50, 50, 10);
	addParam("Degree", 2, 2, 1);
	addParam("Derivatives", 2, 2, 1);*/
	/*mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Osculating);
	addParam("Window Size", 30, 30, 5);
	addParam("Degree", 4, 4, 1);
	addParam("Derivatives", 1, 1, 1);*/
	/*mInterpolator = new ViPolynomialInterpolator();
	addParam("Window Size", 4096, 4096, 5);
	addParam("Degree", 100, 100, 1);*/

	/*mInterpolator = new ViFourierInterpolator(ViFourierInterpolator::Osculating, ViFourierInterpolator::Fixed);
	addParam("Window Size", 10, 100, 10);
	addParam("Degree", 1, 10, 1);
	addParam("Derivatives", 1, 10, 1);*/

	mInterpolator = new ViLagrangeInterpolator();
	addParam("Window Size", 2, 2500, 4);

	/*mInterpolator = new ViNewtonInterpolator();
	addParam("Window Size", 2, 1000, 4);*/

	/*mInterpolator = new ViArimaInterpolator();
	addParam("Window Size", 2048, 2048, 256);
	addParam("AR Degree", 18, 18, 1);
	addParam("I Degree", 0, 0, 1);
	addParam("MA Degree", 3, 3, 1);*/

	/*mInterpolator = new ViGarchInterpolator();
		addParam("Window Size", 2048, 2048, 256);
		addParam("Arch Degree", 1, 1, 1);
		addParam("Garch Degree", 1, 1, 1);*/

	QObject::connect(mInterpolator, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

ViInterpolatorBenchmarker::~ViInterpolatorBenchmarker()
{
}

void ViInterpolatorBenchmarker::progress(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << percentage << "%" << flush;
}

void ViInterpolatorBenchmarker::addParam(QString name, qreal start, qreal end, qreal increase)
{
	if(mInterpolator->hasParameter(name))
	{
		mParamsNames.append(name);
		mParamsStart.append(start);
		mParamsEnd.append(end);
		mParamsIncrease.append(increase);
		mParamsCurrent.append(0);
	}
	else cout << "This interpolator (" << mInterpolator->name("", true).toLatin1().data() << ") doesn't make use of the given parameter (" << name.toLatin1().data() << ")." << endl;
}

void ViInterpolatorBenchmarker::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViInterpolatorBenchmarker::nextParam()
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

void ViInterpolatorBenchmarker::benchmark()
{
	QDir dir("/home/visore/Visore Projects/Files/");
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for(int i = 0; i < dirs.size(); ++i)
	{
		QDir dir3("/home/visore/Visore Projects/Results/" + dirs[i]);
		if(!dir3.exists()) dir3.mkpath(dir3.absolutePath());

		QDir dir2(dir.absoluteFilePath(dirs[i]));
		QStringList files2 = dir2.entryList(QDir::Files);
		for(int j = 0; j < files2.size(); ++j)
		{
			mFiles.enqueue(dir2.absoluteFilePath(files2[j]));
			QString id = ViId::generate();
			mResults.enqueue(dir3.absolutePath() + "/" + mInterpolator->name() + "_" + id + "_ALL.txt");
			mResults2.enqueue(dir3.absolutePath() + "/" + mInterpolator->name() + "_" + id + "_MINIFIED.txt");
		}
	}
	mTotalFiles = mFiles.size();
	nextFile();
}

void ViInterpolatorBenchmarker::nextFile()
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

void ViInterpolatorBenchmarker::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));
	qint64 time;
	QVector<qreal> rmse;
	qreal averageRmse;

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::NoiseMask), mCurrentObject->buffer(ViAudio::Custom));

	do
	{
		for(int i = 0; i < mParamsStart.size(); ++i) mInterpolator->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mInterpolator->validParameters())
		{
			mTime.restart();
			mInterpolator->interpolate(mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::Corrected), mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Custom), rmse, averageRmse);
			time = mTime.elapsed();
			cout << "\r                                                 \r"; // Clear intermidiate percentage
		}
		else
		{
			int maxNoiseSize = ViNoiseCreator::maximumNoiseSize() - ViNoiseCreator::minimumNoiseSize() + 1;
			rmse.resize(maxNoiseSize);
			for(int i = 0; i < maxNoiseSize; ++i) rmse[i] = 2;
			averageRmse = 2;
			time = 0;
		}

		// Write
		/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
		mCurrentObject->encode(ViAudio::Corrected);
		return;*/

		++mDoneParamIterations;
		printFileData(rmse, averageRmse, time);
		printTerminal(rmse, averageRmse, time);
	}
	while(nextParam());

	nextFile();
}

void ViInterpolatorBenchmarker::printFileHeader()
{
	int i;

	mOutputFile.close();
	mOutputFile.setFileName(mResults.dequeue());
	mOutputFile.open(QIODevice::WriteOnly);
	mOutputStream.setDevice(&mOutputFile);

	mOutputStream << mInterpolator->name() << "\n\n";
	mOutputStream << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << "PARAMETER " << i + 1 << " (" << mInterpolator->parameterName(i) <<")\t";
	mOutputStream << "RMSE" << "\t" << "TIME" << "\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream << "NOISE SIZE " << i << " (RMSE)\t";
	mOutputStream << "\n";
	mOutputStream.flush();


	mOutputFile2.close();
	mOutputFile2.setFileName(mResults2.dequeue());
	mOutputFile2.open(QIODevice::WriteOnly);
	mOutputStream2.setDevice(&mOutputFile2);

	mOutputStream2 << mInterpolator->name() << "\n\n";
	mOutputStream2 << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << "PARAMETER " << i + 1 << " (" << mInterpolator->parameterName(i) <<")\t";
	mOutputStream2 << "RMSE" << "\t" << "TIME" << "\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) mOutputStream2 << "NOISE SIZE " << i << " (RMSE)\t";
	mOutputStream2 << "\n";
	mOutputStream2.flush();
}

void ViInterpolatorBenchmarker::printFileData(const QVector<qreal> &rmse, const qreal &averageRmse, const qint64 &time)
{
	int i;

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << (int) mParamsCurrent[i] << "\t";
	mOutputStream << averageRmse << "\t" << time << "\t\t";
	for(i = 0; i < rmse.size(); ++i) mOutputStream << rmse[i] << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();

	if(averageRmse != 2 && time != 0)
	{
		for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << (int) mParamsCurrent[i] << "\t";
		mOutputStream2 << averageRmse << "\t" << time << "\t\t";
		for(i = 0; i < rmse.size(); ++i) mOutputStream2 << rmse[i] << "\t";
		mOutputStream2 << "\n";
		mOutputStream2.flush();
	}
}

void ViInterpolatorBenchmarker::printTerminal(const QVector<qreal> &rmse, const qreal &averageRmse, const qint64 &time)
{
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "RMSE: " << setprecision(6) << averageRmse << "\tTIME: " << time << endl;
}

QString ViInterpolatorBenchmarker::timeConversion(int msecs)
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


void ViInterpolatorBenchmarker::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}
