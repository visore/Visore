#include <vipredictorbenchmarker.h>
#include <viaudiodata.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

#include <viconstantpredictor.h>
#include <vipolynomialpredictor.h>
#include <vihermitepredictor.h>
#include <vifourierpredictor.h>
#include <visplinepredictor.h>
#include <viarmapredictor.h>
#include <vigarchpredictor.h>
#include <vilagrangepredictor.h>

#include <vistandardpolynomial.h>
#include <vifourierpolynomial.h>
#include <vinewtonpolynomial.h>

#define WINDOW_SIZE 4096
#define MAXIMUM_PREDICTION 256

ViPredictorBenchmarker::ViPredictorBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	/*mPredictor = new ViConstantPredictor(ViConstantPredictor::Random);
	addParam("Window Size", 64, 64, 1);*/

	mPredictor = new ViHermitePredictor();
	addParam("Window Size", 1, 10, 1);

	/*mPredictor = new ViPolynomialPredictor(new ViFourierPolynomial(), ViPolynomialPredictor::Fixed);
	addParam("Window Size", 10, 20, 5);
	addParam("Degree", 1, 15, 1);*/

	/*mPredictor = new ViPolynomialPredictor(new ViNewtonPolynomial(), ViPolynomialPredictor::Fixed);
	addParam("Window Size", 10, 10, 5);
	addParam("Degree", 10, 10, 1);*/

	/*mPredictor = new ViLagrangePredictor();
	addParam("Window Size", 1, 128, 1);*/

	//mPredictor = new ViFourierPredictor(ViFourierPredictor::Splines, ViFourierPredictor::Best);
	/*addParam("Window Size", 1, 15, 1);
	addParam("Degree", 1, 10, 1);
	addParam("Derivatives", 5, 5, 1);*/
	/*addParam("Window Size", 1, 15, 1);
	addParam("Degree", 10, 10, 1);
	addParam("Derivatives", 5, 5, 1);*/

	/*mPredictor = new ViSplinePredictor();
	addParam("Window Size", 32, 32, 1);
	addParam("Degree", 1, 1, 1);*/

	/*mPredictor = new ViGarchPredictor();
	addParam("Window Size", 1024, 1024, 1);
	addParam("GARCH Order", 0, 0, 1);
	addParam("ARCH Order", 1, 1, 1);*/

	/*mPredictor = new ViArmaPredictor();
	addParam("Window Size", 256,256, 1);
	addParam("AR Order", 3, 3, 1);
	addParam("I Order", 1, 1, 1);
	addParam("MA Order", 1, 1, 1);*/

	QObject::connect(mPredictor, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

ViPredictorBenchmarker::~ViPredictorBenchmarker()
{
}

void ViPredictorBenchmarker::progress(qreal percentage)
{
	cout << setprecision(2) << percentage << "%" << endl;
}

void ViPredictorBenchmarker::addParam(QString name, qreal start, qreal end, qreal increase)
{
	mParamsNames.append(name);
	mParamsStart.append(start);
	mParamsEnd.append(end);
	mParamsIncrease.append(increase);
	mParamsCurrent.append(0);
}

void ViPredictorBenchmarker::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
}

bool ViPredictorBenchmarker::nextParam()
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

void ViPredictorBenchmarker::benchmark()
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
			mResults.enqueue(dir3.absoluteFilePath(files2[j]) + ViId::generate() + ".txt");
		}
	}
	mTotalFiles = mFiles.size();
	nextFile();
}

void ViPredictorBenchmarker::nextFile()
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

void ViPredictorBenchmarker::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));
	qint64 time;

	do
	{
		for(int i = 0; i < mParamsStart.size(); ++i) mPredictor->setParameter(mParamsNames[i], mParamsCurrent[i]);

		qreal rmse[MAXIMUM_PREDICTION];
		mTime.restart();
		mPredictor->predict(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Custom), MAXIMUM_PREDICTION, rmse);
		time = mTime.elapsed();

		/*ViModelPredictor *p = (ViModelPredictor*) mPredictor;
		QVector<int> r = p->bestDegrees();
		cout<<endl;
		for(int i = 0; i < r.size(); ++i) cout<<r[i]<<"\t";
		cout<<endl;*/

		// Write
		/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
		mCurrentObject->encode(ViAudio::Custom);
		return;*/

		++mDoneParamIterations;
		printFileData(rmse, time);
		printTerminal(rmse, time);
	}
	while(nextParam());

	nextFile();
}

void ViPredictorBenchmarker::printFileHeader()
{
	int i;

	mOutputFile.close();
	mOutputFile.setFileName(mResults.dequeue());
	mOutputFile.open(QIODevice::WriteOnly);
	mOutputStream.setDevice(&mOutputFile);

	mOutputStream << mPredictor->name() << "\n\n";
	mOutputStream << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << "PARAMETER " << i + 1 << " (" << mPredictor->parameterName(i) <<")\t";
	mOutputStream << "RMSE" << "\t" << "TIME" << "\t\t";
	for(i = 1; i <= MAXIMUM_PREDICTION; ++i) mOutputStream << "PREDICTION BY " << i << " (RMSE)\t";
	mOutputStream << "\n";
	mOutputStream.flush();
}

void ViPredictorBenchmarker::printFileData(const qreal *rmse, const qint64 &time)
{
	int i;
	qreal meanRmse = 0;
	for(i = 0; i < MAXIMUM_PREDICTION; ++i) meanRmse += rmse[i];
	meanRmse /= MAXIMUM_PREDICTION;

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << (int) mParamsCurrent[i] << "\t";
	mOutputStream << meanRmse << "\t" << time << "\t\t";
	for(i = 0; i < MAXIMUM_PREDICTION; ++i) mOutputStream << rmse[i] << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();
}

void ViPredictorBenchmarker::printTerminal(const qreal *rmse, const qint64 &time)
{
	int i;
	qreal meanRmse = 0;
	for(i = 0; i < MAXIMUM_PREDICTION; ++i) meanRmse += rmse[i];
	meanRmse /= MAXIMUM_PREDICTION;

	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "RMSE: " << setprecision(6) << meanRmse << "\tTIME: " << time << endl;
}

QString ViPredictorBenchmarker::timeConversion(int msecs)
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


void ViPredictorBenchmarker::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}
