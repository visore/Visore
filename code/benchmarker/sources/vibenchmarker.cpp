#include <vibenchmarker.h>
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

#include <viaudiodata.h>
#include <vifouriertransformer.h>
#include <vipowercalculator.h>
#include <vimath.h>

#define WINDOW_SIZE 4096
#define WRITE false
#define SUMMARY false

ViBenchmarker::ViBenchmarker()
{
	mCurrentObject = ViAudioObject::create();
	mMainTime.start();

	//mInterpolator = new ViPrimitiveInterpolator(ViPrimitiveInterpolator::Lanczos);
	//addParam("Window Size", 2, 1024, 2);
	//addParam("Window Size", 1, 256, 1);

	mInterpolator = new ViPolynomialInterpolator(ViPolynomialInterpolator::Splines);
	addParam("Window Size", 94, 100, 2);
	addParam("Degree", 1, 10, 1);
	//addParam("Derivatives", 1, 10, 1);

//	mInterpolator = new ViNearestNeighbourInterpolator(ViNearestNeighbourInterpolator::Mean);
	//addParam("K", 4,64, 4);
	//addParam("Samples",32,32, 2);

	mInterpolator->setDirection(ViInterpolator::Forward);

	QObject::connect(mInterpolator, SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
}

ViBenchmarker::~ViBenchmarker()
{
}

void ViBenchmarker::progress(qreal percentage)
{
	cout << setprecision(2) << fixed << "\r" << percentage << "%" << flush;
}

void ViBenchmarker::addParam(QString name, qreal start, qreal end, qreal increase)
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

void ViBenchmarker::initParams()
{
	mDoneParamIterations = 0;
	mTotalParamIterations = 1;
	for(int i = 0; i < mParamsStart.size(); ++i)
	{
		mTotalParamIterations *= (mParamsEnd[i] - mParamsStart[i] + mParamsIncrease[i]) / mParamsIncrease[i];
	}
	mTotalParamIterations *= mTotalFiles;
}

bool ViBenchmarker::nextParam()
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

void ViBenchmarker::benchmark(QString folder)
{
	mFiles.append("/home/visore/tar.flac");
	//mFiles.append("/home/visore/corrupted.flac");
	nextFile();
}

void ViBenchmarker::addDir(QString dirName)
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

void ViBenchmarker::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
	{
		for(int i = 0; i < mParamsStart.size(); ++i) mParamsCurrent[i] = mParamsStart[i];
		mCurrentFile = mFiles.dequeue();

//		mAllFile = mResults.dequeue();
//		mMiniFile = mResults2.dequeue();
//
		//printFileHeader(mAllFile);
		//printFileHeader(mMiniFile);

		mCurrentObject->clearBuffers();
		mCurrentObject.setNull();
		mCurrentObject = ViAudioObject::create();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(finished()), this, SLOT(process()));
		mCurrentObject->decode();
	}
}

void ViBenchmarker::process()
{
	/*ViSampleChunk data;
	ViRealSpectrum spectrum(256, mCurrentObject->format(ViAudio::Target), false);

	ViAudioReadData out(mCurrentObject->buffer(ViAudio::Target), true);
	out.setSampleCount(1024);
	out.setWindowFunction("Hamming");

	while(out.hasData())
	{
		out.read();
		//ViSampleChunk &samples = out.splitSamples(0);
		//t.forwardTransform(samples.data(), o);
		spectrum.add(out.frequencies());

	}

	spectrum.update();
	for(int i = 0; i < spectrum.size()/2;++i)
	{
		cout << QString::number(spectrum[i].polar().decibel().real(), 'f', 10).toLatin1().data() << endl;
	}

	quit();*/




	ViSampleChunk data;
	ViRealSpectrum spectrum(4096, mCurrentObject->format(ViAudio::Target), false);

	ViFourierTransformer t;
	t.setSize(4096);

	ViAudioReadData out(mCurrentObject->buffer(ViAudio::Target), true);
	out.setSampleCount(4096);
	out.setWindowFunction("Hamming");

	qreal o[4096];

	qreal oo[4096];
	for(int i = 0; i < 4096;++i) oo[i]=0;
	int c = 0;

	while(out.hasData())
	{
		out.read();
		ViSampleChunk &samples = out.samples();
		t.forwardTransform(samples.data(), o);
		for(int i = 0; i < 4096;++i) oo[i]+=o[i];
		++c;
	}

	for(int i = 0; i < 4096;++i) oo[i] /= c;

	/*qreal value = 0;
	for(int i = 1; i < 512; ++i) value += abs(oo[i - 1] - oo[i]);
	cout<<value/512<<endl;*/

	qreal value = 0;
	int ttt = 0;
	for(int i = 1; i <2048; ++i)
	{
		cout<<QString::number(abs(oo[i - 1] - oo[i]), 'f', 10).toLatin1().data()<<endl;

		/*if(i >= 1024)
	{*/
		++ttt;
		value+=abs(oo[i - 1] - oo[i]);
		//}
	}
cout<<"ppppppp: "<<value/ttt<<endl;
	quit();
}

void ViBenchmarker::printFileHeader(QString filepath)
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

void ViBenchmarker::printFileDataAll(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
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

void ViBenchmarker::printFileDataMinified(QString filepath, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
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

void ViBenchmarker::printFileDataSummary(QString genre, ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
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

QString ViBenchmarker::genre(QString path)
{
	path = path.replace("/home/visore/Visore Projects/Files/", "");
	int i = path.indexOf("/");
	if(i < 0) return "";
	return path.left(i).replace("/", "");
}

void ViBenchmarker::printTerminal(ViErrorCollection &interpolationErrors, ViErrorCollection &modelErrors, const qint64 &time)
{
	qreal percentageDone = mDoneParamIterations / qreal(mTotalParamIterations);
	qint64 remaining = mMainTime.elapsed();
	remaining = ((1.0 / percentageDone) * remaining) - remaining;

	qreal best = interpolationErrors.nrmse();
	if(best >=0 && best < mBestScore) mBestScore = best;

	cout << int(percentageDone * 100.0) << "%\t(" << timeConversion(remaining).toLatin1().data() << ")\t";
	cout << "INTERPOLATION NRMSE: " << setprecision(6) << interpolationErrors.nrmse() << " ("  << setprecision(6) << mBestScore << ")\tMODEL NRMSE: " << setprecision(6) << modelErrors.nrmse() << "\tTIME: " << time << endl;
}

QString ViBenchmarker::timeConversion(int msecs)
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


void ViBenchmarker::quit()
{
	++mQuitCount;
	if(WRITE && mQuitCount < 2) return;

	cout << "QUIT!" << endl;
	exit(0);
}
