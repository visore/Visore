#include <vipredictorbenchmarker.h>
#include <viaudiodata.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

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
#define MAXIMUM_PREDICTION 50

ViPredictorBenchmarker::ViPredictorBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	/*mPredictor = new ViConstantPredictor(ViConstantPredictor::Last);
	addParam("Window Size", 64, 64, 1);*/

	/*mPredictor = new ViHermitePredictor();
	addParam("Window Size", 1, 256, 1);*/

	/*mPredictor = new ViLagrangePredictor();
	addParam("Window Size", 1, 128, 1);*/

	/*mPredictor = new ViPolynomialPredictor(ViPolynomialPredictor::Normal, ViPolynomialPredictor::Fixed);
	addParam("Window Size", 8, 1024, 8);
	addParam("Degree", 1, 10, 1);
	addParam("Derivatives", 1, 4, 1);*/

	/*mPredictor = new ViFourierPredictor(ViFourierPredictor::Normal, ViFourierPredictor::Fixed);
	addParam("Window Size", 8, 64, 8);
	addParam("Degree", 1, 10, 1);
	addParam("Derivatives", 1, 4, 1);*/

	/*addParam("Window Size", 1, 15, 1);
	addParam("Degree", 10, 10, 1);
	addParam("Derivatives", 5, 5, 1);*/

	/*mPredictor = new ViGarchPredictor();
	addParam("Window Size", 1024, 1024, 1);
	addParam("GARCH Order", 0, 0, 1);
	addParam("ARCH Order", 1, 1, 1);*/

	/*mPredictor = new ViArmaPredictor();
	addParam("Window Size", 32,512, 32);
	addParam("AR Degree", 1, 7, 1);
	//addParam("I Degree", 0, 1, 1);
	addParam("MA Degree", 0, 1, 1);*/
	/*addParam("Window Size", 64,64, 32);
		addParam("AR Degree", 1, 7, 1);
		//addParam("I Degree", 0, 1, 1);
		addParam("MA Degree", 0, 1, 1);*/

	mPredictor = new ViNeuralPredictor();
	addParam("Window Size", 1, 1, 1);
	addParam("l1", 0, 0, 2);
	//addParam("l3", 15,15, 5);

	QObject::connect(mPredictor, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

ViPredictorBenchmarker::~ViPredictorBenchmarker()
{
}

void ViPredictorBenchmarker::progress(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << percentage << "%" << flush;
}

void ViPredictorBenchmarker::addParam(QString name, qreal start, qreal end, qreal increase)
{
	if(mPredictor->hasParameter(name))
	{
		mParamsNames.append(name);
		mParamsStart.append(start);
		mParamsEnd.append(end);
		mParamsIncrease.append(increase);
		mParamsCurrent.append(0);
	}
	else cout << "This predictor (" << mPredictor->name("", true).toLatin1().data() << ") doesn't make use of the given parameter (" << name.toLatin1().data() << ")." << endl;
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

void ViPredictorBenchmarker::benchmark(QString folder)
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

void ViPredictorBenchmarker::addDir(QString dirName)
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
		mResults.enqueue(dirResult.absolutePath() + "/" + mPredictor->name() + "_" + id + "_ALL.txt");
		mResults2.enqueue(dirResult.absolutePath() + "/" + mPredictor->name() + "_" + id + "_MINIFIED.txt");
	}
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
	ViErrorCollection predictionErrors;
	ViError modelError;

	do
	{
		predictionErrors.clear();
		modelError.clear();
		for(int i = 0; i < mParamsStart.size(); ++i) mPredictor->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mPredictor->validParameters())
		{
			mTime.restart();
			mPredictor->predict(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Custom), MAXIMUM_PREDICTION, &predictionErrors, &modelError);
			time = mTime.elapsed();
			cout << "\r                                                 \r"; // Clear intermidiate percentage
		}
		else
		{
			time = 0;
		}

		// Write
		/*QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
		mCurrentObject->encode(ViAudio::Custom);
		return;*/

		/*ViAudioWriteData d(mCurrentObject->buffer(ViAudio::CustomMask));
		ViSampleChunk f(4096);
		QList<qreal> errors = predictionErrors.at(1).nrmseTime();
		while(true)
		{
			if(errors.size()<4096)break;
			for(int i = 0; i < 4096; ++i)
			{
				f[i] = errors[0];
				errors.removeFirst();
			}
			d.write(f);
		}
		mCurrentObject->encode(ViAudio::CustomMask);
		return;*/

		++mDoneParamIterations;
		printFileData(predictionErrors, modelError, time);
		printTerminal(predictionErrors, modelError, time);
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
	mOutputStream << "NRMSE PREDICTION" << "\t" << "NRMSE MODEL" << "\t" << "TIME" << "\t\t";
	for(i = 1; i <= MAXIMUM_PREDICTION; ++i) mOutputStream << "PREDICTION BY " << i << " (NRMSE PREDICTION)\t";
	mOutputStream << "\n";
	mOutputStream.flush();


	mOutputFile2.close();
	mOutputFile2.setFileName(mResults2.dequeue());
	mOutputFile2.open(QIODevice::WriteOnly);
	mOutputStream2.setDevice(&mOutputFile2);

	mOutputStream2 << mPredictor->name() << "\n\n";
	mOutputStream2 << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << "PARAMETER " << i + 1 << " (" << mPredictor->parameterName(i) <<")\t";
	mOutputStream2 << "NRMSE PREDICTION" << "\t" << "NRMSE MODEL" << "\t" << "TIME" << "\t\t";
	for(i = 1; i <= MAXIMUM_PREDICTION; ++i) mOutputStream2 << "PREDICTION BY " << i << " (NRMSE PREDICTION)\t";
	mOutputStream2 << "\n";
	mOutputStream2.flush();
}

void ViPredictorBenchmarker::printFileData(ViErrorCollection &predictionErrors, ViError &modelError, const qint64 &time)
{
	int i;

	for(i = 0; i < mParamsStart.size(); ++i) mOutputStream << (int) mParamsCurrent[i] << "\t";
	mOutputStream << predictionErrors.nrmse() << "\t" << modelError.nrmse() << "\t" << time << "\t\t";
	for(i = 1; i <= MAXIMUM_PREDICTION; ++i) mOutputStream << predictionErrors.at(i).nrmse() << "\t";
	mOutputStream << "\n";
	mOutputStream.flush();

	mOutputStream << "\n\n\n";
	QList<qreal> errors = predictionErrors.at(1).nrmseTime();
	for(i = 0; i < errors.size(); ++i) mOutputStream << errors[i] << "\n";
	mOutputStream.flush();

	if(predictionErrors.nrmse() >= 0 && time != 0)
	{
		for(i = 0; i < mParamsStart.size(); ++i) mOutputStream2 << (int) mParamsCurrent[i] << "\t";
		mOutputStream2 << predictionErrors.nrmse() << "\t" << modelError.nrmse() << "\t" << time << "\t\t";
		for(i = 1; i <= MAXIMUM_PREDICTION; ++i) mOutputStream2 << predictionErrors.at(i).nrmse() << "\t";
		mOutputStream2 << "\n";
		mOutputStream2.flush();
	}
}

void ViPredictorBenchmarker::printTerminal(ViErrorCollection &predictionErrors, ViError &modelError, const qint64 &time)
{	
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "PREDICTION NRMSE: " << setprecision(6) << predictionErrors.nrmse() << "\tMODEL NRMSE: " << setprecision(6) << modelError.nrmse() << "\tTIME: " << time << endl;
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
