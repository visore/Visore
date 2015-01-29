#include <vistatistician.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <QTextStream>
#include <QDir>
#include <iomanip>

ViStatistician::ViStatistician()
{
	mCurrentObject = ViAudioObject::create();
	mValue = 0;
	mCount = 0;
	mValuePos = 0;
	mCountPos = 0;
	mValueNeg = 0;
	mCountNeg = 0;

	mMode = Pearson;
	mWindowSize = 32;
}

ViStatistician::~ViStatistician()
{
}

void ViStatistician::run(QString folder)
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

	nextFile();
}

void ViStatistician::addDir(QString dirName)
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
	}

	QString name = "";
	if(mMode == Mean) name = "mean";
	else if(mMode == StandardDeviation) name = "standarddeviation";
	else if(mMode == Pearson) name = "pearson";
	mFile = dirResult.absolutePath() + "/summary" + name + ".txt";
}

void ViStatistician::nextFile()
{
	if(mFiles.isEmpty())
	{
		if(mMode == Pearson) printFileFinalPearson();
		else printFileFinal();
		quit();
	}
	else
	{
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

void ViStatistician::process()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process()));

	ViAudioReadData data(mCurrentObject->buffer(ViAudio::Target));
	data.setSampleCount(mWindowSize);
	qreal value = 0;
	qint64 count = 0;

	qreal top = 0, bottom1 = 0, bottom2 = 0;
	qreal valuePos = 0, valueNeg = 0;
	int countPos = 0, countNeg = 0;

	while(data.hasData())
	{
		data.read();
		ViSampleChunk &samples1 = data.splitSamples(0);
		ViSampleChunk &samples2 = data.splitSamples(1);

		if(mMode == Mean)
		{
			value += mean(samples1);
			value += mean(samples2);
			count += 2;
		}
		else if(mMode == StandardDeviation)
		{
			value += standardDeviation(samples1);
			value +standardDeviation(samples2);
			count += 2;
		}
		else if(mMode == Pearson)
		{
			qreal theTopA = 0, theBottom1A = 0, theBottom2A = 0;
			qreal theTopB = 0, theBottom1B = 0, theBottom2B = 0;

			qreal value1 = pearson(samples1, theTopA, theBottom1A, theBottom2A);
			qreal value2 = pearson(samples2, theTopB, theBottom1B, theBottom2B);
			/*if(value1 != 0)
			{
				top += theTopA;
				bottom1 += theBottom1A;
				bottom2 += theBottom2A;
			}
			if(value2 != 0)
			{
				top += theTopB;
				bottom1 += theBottom1B;
				bottom2 += theBottom2B;
			}*/
			if(value1 > 0)
			{
				valuePos += value1;
				++countPos;
			}
			else if(value1 < 0)
			{
				valueNeg += value1;
				++countNeg;
			}
			if(value2 > 0)
			{
				valuePos += value2;
				++countPos;
			}
			else if(value2 < 0)
			{
				valueNeg += value2;
				++countNeg;
			}
		}
	}

	if(mMode == Pearson)
	{
		/*value = top / (qSqrt(bottom1) * qSqrt(bottom2));
		mValue += value;
		mCount += 1;

		printFileData(value);
		printTerminal(value);*/


		mValuePos += valuePos;
		mCountPos += countPos;
		mValueNeg += valueNeg;
		mCountNeg += countNeg;

		valuePos /= countPos;
		valueNeg /= countNeg;

		printFileDataPearson(valuePos, valueNeg);
		printTerminalPearson(valuePos, valueNeg);
	}
	else
	{
		mValue += value;
		mCount += count;
		value /= count;
		printFileData(value);
		printTerminal(value);
	}

	nextFile();
}

qreal ViStatistician::mean(ViSampleChunk &data)
{
	int size = data.size();
	qreal value = 0;
	for(int i = 0; i < size; ++i)
	{
		value += data[i];
	}
	return value / size;
}

qreal ViStatistician::standardDeviation(ViSampleChunk &data)
{
	int i, size = data.size();

	qreal mean = 0;
	for(i = 0; i < size; ++i)
	{
		mean += data[i];
	}
	mean /= size;

	qreal deviation = 0;
	for(i = 0; i < size; ++i)
	{
		deviation += qPow(data[i] - mean, 2);
	}

	return qSqrt(deviation / (size - 1));
}

qreal ViStatistician::pearson(ViSampleChunk &data, qreal &theTop, qreal &theBottom1, qreal &theBottom2)
{
	int i;
	int size = data.size();
	int n = qFloor(size / 2.0);

	qreal mean1 = 0, mean2= 0;
	for(i = 0; i < n; ++i)
	{
		mean1 += data[i];
		mean2 += data[n+ i];
	}
	mean1 /= n;
	mean2 /= n;

	qreal top = 0;
	qreal bottom1 = 0;
	qreal bottom2 = 0;
	for(i = 0; i < n; ++i)
	{
		top += (data[i] - mean1) * (data[n + i] - mean2);
		bottom1 += qPow((data[i] - mean1), 2);
		bottom2 += qPow((data[n + i] - mean2), 2);
	}

	theTop += top;
	theBottom1 = bottom1;
	theBottom2 = bottom2;

	qreal r = top / (qSqrt(bottom1) * qSqrt(bottom2));
	if(qIsNaN(r)) return 0;
	return r;
}

void ViStatistician::printFileData(double value)
{
	QFile file(mFile);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	stream << QString::number(value, 'f', 10) << "\n";

	stream.flush();
	file.close();
}

void ViStatistician::printFileDataPearson(double value1, double value2)
{
	QFile file(mFile);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	stream << QString::number(value1, 'f', 10) << "\t" << QString::number(value2, 'f', 10) << "\n";

	stream.flush();
	file.close();
}

void ViStatistician::printFileFinal()
{
	QFile file(mFile);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	stream << "\n" << QString::number((mValue / mCount), 'f', 10) << "\n";

	stream.flush();
	file.close();
}

void ViStatistician::printFileFinalPearson()
{
	QFile file(mFile);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream stream(&file);

	stream << "\n" << QString::number((mValuePos / mCountPos), 'f', 10) << "\t" << QString::number((mValueNeg / mCountNeg), 'f', 10) << "\n";

	stream.flush();
	file.close();
}

void ViStatistician::printTerminal(double value)
{
	static int count = 0;
	++count;
	cout << count << "\t" << setprecision(6) << value << "\t(" << (mValue / mCount) << ")" << endl;
}

void ViStatistician::printTerminalPearson(double value1, double value2)
{
	static int count = 0;
	++count;
	cout << count << "\t" << setprecision(6) << value1 << "\t(" << (mValuePos / mCountPos) << ")" << "\t\t" << value2 << "\t(" << (mValueNeg / mCountNeg) << ")" << endl;
}

void ViStatistician::quit()
{
	cout << "QUIT!" << endl;
	exit(0);
}

