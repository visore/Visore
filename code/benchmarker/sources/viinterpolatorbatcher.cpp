#include <viinterpolatorbatcher.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

#include <vinearestneighbourinterpolator.h>
#include <viprimitiveinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <vifourierinterpolator.h>
#include <vilagrangeinterpolator.h>
#include <vinewtoninterpolator.h>
#include <viarimainterpolator.h>
#include <vigarchinterpolator.h>
#include <vihermiteinterpolator.h>
#include <vineuralinterpolator.h>

#define WINDOW_SIZE 4096
#define WRITE false
#define SUMMARY true

ViInterpolatorBatcher::ViInterpolatorBatcher()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	// ***************************
	// PRIMITIVE
	// ***************************

	// Zero Interpolator
	//mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Zero);

	// Cosine Interpolator
	//mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Cosine);

	// Random Interpolator
	/*mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Random);
	setParam("Window Size", 2);*/

	// Mirror Interpolator
	//mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Mirror);

	// Duplicate Interpolator
	//mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Duplicate);

	// Similarity Interpolator
	/*mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Similarity);
	setParam("Window Size", 2);
	setParam("Samples", 2);*/

	// Lanczos Interpolator
	/*mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Lanczos);
	setParam("Window Size", 2);*/


	// ***************************
	// NN
	// ***************************

	/*mInterpolator = new ViNearestNeighbourInterpolator(ViNearestNeighbourInterpolator::Traditional);
	setParam("K", 2);*/


	// ***************************
	// LAGRANGE
	// ***************************

	/*mInterpolator = new ViLagrangeInterpolator();
	setParam("Window Size", 2);*/


	// ***************************
	// NEWTON
	// ***************************

	/*mInterpolator = new ViNewtonInterpolator();
	setParam("Window Size", 2);*/


	// ***************************
	// HERMITE
	// ***************************

	/*mInterpolator = new ViHermiteInterpolator();
	setParam("Window Size", 2);*/


	// ***************************
	// Polynomial
	// ***************************

	// Normal
	/*mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Normal);
	setParam("Window Size", 2);
	setParam("Degree", 2);*/

	// Osculating
	/*mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Osculating);
	setParam("Window Size", 2);
	setParam("Degree", 2);
	setParam("Derivatives", 2);*/

	// Splines
	/*mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Splines);
	setParam("Window Size", 8);
	setParam("Degree", 1);*/


	// ***************************
	// Fourier
	// ***************************

	// Normal
	/*mInterpolator = new ViFourierInterpolator(ViFourierInterpolator::Normal);
	setParam("Window Size", 2);
	setParam("Degree", 2);*/

	// Osculating
	/*mInterpolator = new ViFourierInterpolator(ViFourierInterpolator::Osculating);
	setParam("Window Size", 2);
	setParam("Degree", 2);
	setParam("Derivatives", 2);*/

	// Splines
	/*mInterpolator = new ViFourierInterpolator(ViFourierInterpolator::Splines);
	setParam("Window Size", 2);
	setParam("Degree", 2);*/


	// ***************************
	// AR
	// ***************************

	/*mInterpolator = new ViArimaInterpolator();
	setParam("Window Size", 1024);
	setParam("AR Degree", 2);
	setParam("I Degree", 0);
	setParam("MA Degree", 0);*/


	// ***************************
	// MA
	// ***************************

	/*mInterpolator = new ViArimaInterpolator();
	setParam("Window Size", 1024);
	setParam("AR Degree", 0);
	setParam("I Degree", 0);
	setParam("MA Degree", 2);*/


	// ***************************
	// ARMA
	// ***************************

	/*mInterpolator = new ViArimaInterpolator(ViArimaInterpolator::AR2);
	setParam("Window Size", 1440);
	setParam("AR Degree", 9);*/
	//setParam("I Degree", 0);
	//setParam("MA Degree", 2);


	// ***************************
	// ARIMA
	// ***************************

	/*mInterpolator = new ViArimaInterpolator();
	setParam("Window Size", 1024);
	setParam("AR Degree", 2);
	setParam("I Degree", 2);
	setParam("MA Degree", 2);*/


	// ***************************
	// ARCH
	// ***************************

	/*mInterpolator = new ViGarchInterpolator();
	setParam("Window Size", 1024);
	setParam("Arch Degree", 2);
	setParam("Garch Degree", 0);*/


	// ***************************
	// GARCH
	// ***************************

	/*mInterpolator = new ViGarchInterpolator();
	setParam("Window Size", 1024);
	setParam("Arch Degree", 2);
	setParam("Garch Degree", 2);*/

	mInterpolator = new ViNeuralInterpolator(ViNeuralInterpolator::Interpolation);
	//addParam("Hidden Layer 3", 0, 32, 4);
	//addParam("Hidden Layer 2", 60, 60, 4);
//	addParam("Hidden Layer 1", 128, 128, 4);
	addParam("Window Size", 128, 128, 1);

	mInterpolator->setDirection(ViInterpolator::Forward);
	QObject::connect(mInterpolator, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

ViInterpolatorBatcher::~ViInterpolatorBatcher()
{
}

void ViInterpolatorBatcher::progress(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << percentage << "%" << flush;
}

void ViInterpolatorBatcher::addParam(QString name, qreal start, qreal end, qreal increase)
{
	if(start == 0 && end == 0) return;
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

void ViInterpolatorBatcher::setParam(QString name, qreal value)
{
	addParam(name, value, value, 0);
}

void ViInterpolatorBatcher::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
	mTotalParamIterations *= mTotalFiles;
}

bool ViInterpolatorBatcher::nextParam()
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

void ViInterpolatorBatcher::benchmark(QString folder)
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
	initParams();
	nextFile();
}

void ViInterpolatorBatcher::addDir(QString dirName)
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
		mResults.enqueue(dirResult.absolutePath() + "/" + mInterpolator->name() + "_" + id + "_ALL.txt");
		mResults2.enqueue(dirResult.absolutePath() + "/" + mInterpolator->name() + "_" + id + "_MINIFIED.txt");
	}
}

void ViInterpolatorBatcher::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
	{
		for(int i = 0; i < mParamsStart.size(); ++i) mParamsCurrent[i] = mParamsStart[i];
		mCurrentFile = mFiles.dequeue();

		mAllFile = mResults.dequeue();
		mMiniFile = mResults2.dequeue();

		printFileHeader(mAllFile);
		printFileHeader(mMiniFile);

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(process()));
		mCurrentObject->decode();
	}
}

void ViInterpolatorBatcher::process()
{
	mQuitCount = 0;
	mBestScore = DBL_MAX;

	QObject::disconnect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(process()));
	qint64 time;

	ViErrorCollection interpolationErrors, modelErrors;

	ViNoiseCreator creator;
	creator.createNoise(mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::NoiseMask), mCurrentObject->buffer(ViAudio::Custom));

	//do
	//{
		mQuitCount = 0;
		interpolationErrors.clear();
		modelErrors.clear();

		for(int i = 0; i < mParamsStart.size(); ++i) mInterpolator->setParameter(mParamsNames[i], mParamsCurrent[i]);

		if(mInterpolator->validParameters())
		{
			mTime.restart();
			mInterpolator->interpolate(mCurrentObject->buffer(ViAudio::Corrupted), mCurrentObject->buffer(ViAudio::Corrected), mCurrentObject->buffer(ViAudio::Target), mCurrentObject->buffer(ViAudio::Custom), &interpolationErrors, &modelErrors);
			time = mTime.elapsed();
			cout << "\r                                                 \r"; // Clear intermidiate percentage
		}
		else
		{
			time = 0;
		}

		// Write
		if(WRITE)
		{
			QObject::connect(mCurrentObject.data(), SIGNAL(encoded()), this, SLOT(quit()));
			mCurrentObject->encode(ViAudio::Corrupted);
			//return;
		}

		++mDoneParamIterations;
		printFileDataAll(mAllFile, interpolationErrors, modelErrors, time);
		printFileDataMinified(mMiniFile, interpolationErrors, modelErrors, time);
		if(SUMMARY) printFileDataSummary(genre(mMiniFile), interpolationErrors, modelErrors, time);
		printTerminal(interpolationErrors, modelErrors, time);
	//}
	//while(nextParam());

	nextFile();
}

void ViInterpolatorBatcher::printFileHeader(QString filepath)
{
	int i;

	QFile file(filepath);
	file.open(QIODevice::WriteOnly);
	QTextStream stream(&file);

	stream << mInterpolator->name() << "\n\n";
	stream << QFileInfo(mCurrentFile).fileName() << "\n\n";

	for(i = 0; i < mParamsStart.size(); ++i) stream << "PARAMETER " << i + 1 << " (" << mInterpolator->parameterName(i) <<")\t";
	stream << "NRMSE INTERPOLATION" << "\t" << "NRMSE MODEL" << "\t" << "TIME" << "\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << "NOISE SIZE " << i << " (NRMSE INTERPOLATION)\t";
	stream << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << "NOISE SIZE " << i << " (NRMSE MODEL)\t";
	stream << "\n";
	stream.flush();

	file.close();
}

void ViInterpolatorBatcher::printFileDataAll(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
{
	int i;

	QFile file(filepath);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	for(i = 0; i < mParamsStart.size(); ++i) stream << (int) mParamsCurrent[i] << "\t";
	stream << interpolationErrors.nrmse() << "\t" << modelErrors.nrmse() << "\t" << time << "\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << interpolationErrors[i].nrmse() << "\t";
	stream << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << modelErrors[i].nrmse() << "\t";
	stream << "\n";
	stream.flush();
}

void ViInterpolatorBatcher::printFileDataMinified(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
{
	int i;

	if(interpolationErrors.nrmse() >= 0 && time != 0)
	{
		QFile file(filepath);
		file.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream stream(&file);

		for(i = 0; i < mParamsStart.size(); ++i) stream << (int) mParamsCurrent[i] << "\t";
		stream << interpolationErrors.nrmse() << "\t" << modelErrors.nrmse() << "\t" << time << "\t\t";
		for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << interpolationErrors[i].nrmse() << "\t";
		stream << "\t";
		for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << modelErrors[i].nrmse() << "\t";
		stream << "\n";
		stream.flush();
	}
}

void ViInterpolatorBatcher::printFileDataSummary(QString genre, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
{
	int i;

	QString filepath = "";
	for(i = 0; i < mSummaryFiles.size(); ++i)
	{
		if(mSummaryFiles[i].contains(genre))
		{
			filepath = mSummaryFiles[i];
			break;
		}
	}
	if(filepath == "") return;

	QFile file(filepath);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	for(i = 0; i < mParamsStart.size(); ++i) stream << (int) mParamsCurrent[i] << "\t";
	stream << interpolationErrors.nrmse() << "\t" << modelErrors.nrmse() << "\t" << time << "\t\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << interpolationErrors[i].nrmse() << "\t";
	stream << "\t";
	for(i = ViNoiseCreator::minimumNoiseSize(); i <= ViNoiseCreator::maximumNoiseSize(); ++i) stream << modelErrors[i].nrmse() << "\t";
	stream << "\n";
	stream.flush();
}

QString ViInterpolatorBatcher::genre(QString path)
{
	path = path.replace("/home/visore/Visore Projects/Files/", "");
	int i = path.indexOf("/");
	if(i < 0) return "";
	return path.left(i).replace("/", "");
}

void ViInterpolatorBatcher::printTerminal(ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
{
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	qreal best = interpolationErrors.nrmse();
	if(best >=0 && best < mBestScore) mBestScore = best;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "INTERPOLATION NRMSE: " << setprecision(12) << interpolationErrors.nrmse() << " ("  << setprecision(6) << mBestScore << ")\tMODEL NRMSE: " << setprecision(6) << modelErrors.nrmse() << "\tTIME: " << time << endl;
}

QString ViInterpolatorBatcher::timeConversion(int msecs)
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


void ViInterpolatorBatcher::quit()
{
	++mQuitCount;
	if(WRITE && mQuitCount < 2) return;

	cout << "QUIT!" << endl;
	exit(0);
}
