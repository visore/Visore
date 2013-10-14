#include <vibenchmarker.h>
#include <viaudiodata.h>
#include <vipiecewiseconstantinterpolator.h>
#include <visplineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <vitriginterpolator.h>
#include <vihermiteinterpolator.h>
#include <vilogger.h>

#include <iomanip>

#define WINDOW_SIZE 2048
#define NOISE_SIZE 32
#define MAX_NOISE_SIZE 150
#define MAX_LENGTH 10000000 // Maximum length of song to process, 10000000 samples is about 2 mins

ViBenchMarker::ViBenchMarker()
{
	mInterpolator = NULL;
}

ViBenchMarker::~ViBenchMarker()
{
	clear();
}

void ViBenchMarker::benchmark(QString path)
{
	clear();
	mFiles.append(path);
	mOriginalFiles = mFiles;
	cout << "Benchmarking started (1 file)" << endl;
	benchmark();
}

void ViBenchMarker::benchmark(QStringList paths)
{
	clear();
	mFiles = paths;
	mOriginalFiles = mFiles;
	cout << "Benchmarking started (" << paths.size() << " files)" << endl;

	for(int i = 1; i <= MAX_NOISE_SIZE; ++i)
	{
		mNoises.append(createNoise(i));
		mInfos.append(QList<ViBenchMarkInfo>());
	}

	benchmark();
}

void ViBenchMarker::clear()
{
	if(mInterpolator != NULL)
	{
		delete mInterpolator;
		mInterpolator = NULL;
	}
	mFile = "";
	mCurrentNoiseSize = 1;
	mInfos.clear();
	mFiles.clear();
	mOriginalFiles.clear();
	mNoises.clear();
}

void ViBenchMarker::benchmark()
{
	//createNoise(mCurrentNoiseSize);

	//mInterpolator = new ViPiecewiseConstantInterpolator();
	//mInterpolator = new ViPolynomialInterpolator(1);
	mInterpolator = new ViSplineInterpolator(2);

	mTime.start();
	processNext();
}

void ViBenchMarker::processNext()
{
	if(mFiles.isEmpty())
	{
			for(int j = 0; j < MAX_NOISE_SIZE; ++j)
			{
				qreal average = 0;
				qint64 count = 0;

				QHash<QString,qreal> averages;
				QHash<QString,qreal> counts;
				averages["classical"] = counts["classical"] = 0;
				averages["country"] = counts["country"] = 0;
				averages["jazz"] = counts["jazz"] = 0;
				averages["metal"] = counts["metal"] = 0;
				averages["pop"] = counts["pop"] = 0;
				averages["raggae"] = counts["raggae"] = 0;
				averages["rock"] = counts["rock"] = 0;
				averages["trance"] = counts["trance"] = 0;

				cout << j+1 << "\t";
				for(int i = 0; i < mInfos[j].size(); ++i)
				{
					average += mInfos[j][i].mAccuracy;
					count += 1;

					QString type = "";
					if(mInfos[j][i].mFile.contains("/classical/")) type = "classical";
					else if(mInfos[j][i].mFile.contains("/country/")) type = "country";
					else if(mInfos[j][i].mFile.contains("/jazz/")) type = "jazz";
					else if(mInfos[j][i].mFile.contains("/metal/")) type = "metal";
					else if(mInfos[j][i].mFile.contains("/pop/")) type = "pop";
					else if(mInfos[j][i].mFile.contains("/raggae/")) type = "raggae";
					else if(mInfos[j][i].mFile.contains("/rock/")) type = "rock";
					else if(mInfos[j][i].mFile.contains("/trance/")) type = "trance";
					averages[type] += mInfos[j][i].mAccuracy;
					counts[type] += 1;
				}

				QStringList keys = averages.keys();
				qSort(keys);
				foreach(QString key, keys)
				{
					if(counts[key] != 30)
					{
						//LOG("Not all songs were processed. Only " + QString::number(counts[key]) + " were.", QtFatalMsg);
					}
					cout << setprecision(10) << averages[key] / qreal(counts[key]) << "\t";
				}
				cout << average / qreal(count) <<endl;
			}

		cout << "Benchmarking finished" <<endl;
		cout << "Time ellapsed: "<<mTime.elapsed()/1000<<" seconds"<<endl;
		quit();
		return;
	}
	mFile = mFiles.takeFirst();
	//cout << "Processing next track: " << mFile.toLatin1().data() << endl;
	mBuffer.clear();
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(process()));
	mCoder.decode(mFile, &mBuffer);
}

void ViBenchMarker::process()
{
	QObject::disconnect(&mCoder, SIGNAL(finished()), this, SLOT(process()));

	ViAudioReadData data(&mBuffer);
	data.setSampleCount(WINDOW_SIZE);

	int i, j, current = 0, total = MAX_LENGTH; //data.bufferSamples();
	int oldProgress = 0;
	qreal progress = 0;

	QList<qreal> differences;
	QList<qint64> counts;
	for(int i = 0; i < MAX_NOISE_SIZE;++i)
	{
		differences.append(0);
		counts.append(0);
	}

	qint64 currentSamples = 0;
	while(data.hasData())
	{
		data.read();
		if(data.samples().size() != WINDOW_SIZE) break;

		currentSamples += data.sampleCount();
		if(currentSamples > MAX_LENGTH) break;		/*if(mCurrentNoiseSize < MAX_NOISE_SIZE)
		{*/
			//mFiles = mOriginalFiles;


		ViSampleChunk &read1 = data.splitSamples(0);
		ViSampleChunk &read2 = data.splitSamples(1);

		ViSampleChunk write1, write2;
		for(i = 0; i < MAX_NOISE_SIZE;++i)
		{
			ViNoise &noise = mNoises[i];

			write1 = data.splitSamples(0);
			write2 = data.splitSamples(1);

			mInterpolator->interpolate(write1, noise);
			mInterpolator->interpolate(write2, noise);

			for(j = 0; j < noise.size(); ++j)
			{
				if(noise[j])
				{
					differences[i] += qAbs(read1[j] - write1[j]) / 2.0; // Devide by 2 to get the range in [0, 1]
					differences[i] += qAbs(read2[j] - write2[j]) / 2.0;
					counts[i] += 2;
				}
			}
		}

		current += (read1.size() + read2.size());
		progress = (current / float(total));
		progress /= mOriginalFiles.size();
		progress += (mOriginalFiles.size() - mFiles.size() - 1) / qreal(mOriginalFiles.size());
		progress *= 100;
		if(qFloor(progress) > oldProgress)
		{
			oldProgress = progress;
			cout << "Progress: " << int(progress) << "%\t(" <<mTime.elapsed()/1000<<" secs)"  << endl;
		}
	}

	for(i = 0; i < MAX_NOISE_SIZE;++i)
	{
		ViBenchMarkInfo info;
		info.mFile = mFile;
		info.mDifferences = differences[i];
		info.mCount = counts[i];
		info.mAccuracy = (1 - (differences[i] / (counts[i]  * 2.0)));
		mInfos[i].append(info);
		//cout<<i+1<<":\t"<<differences[i]<<" "<<counts[i]<<" "<<info.mAccuracy<<endl;
		//cout << "Track accuracy: " << setprecision(10) << info.mAccuracy * 100 << "%" <<endl;
	}
	processNext();
}

ViNoise ViBenchMarker::createNoise(int length)
{
	ViNoise noise;

	int min = 128; // Minimum samples on both sides of noise

	noise.resize(WINDOW_SIZE / 2);
	int unit = (min * 2) - (length / 2);
	int parts = qFloor(noise.size() / float(unit));

	for(int i = 1; i < parts; ++i)
	{
		int start = i * unit;
		int end = start + length;
		for(int j = start; j < end; ++j)
		{
			noise.set(j, 1);
		}
	}
	return noise;
}

void ViBenchMarker::quit()
{
	int x = 0;
	int y = 1 / x;
}
