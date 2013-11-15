#include <vibenchmarker.h>
#include <viaudiodata.h>
#include <vipiecewiseconstantinterpolator.h>
#include <visplineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <vicosineinterpolator.h>
#include <vifourierinterpolator.h>
#include <vihermiteinterpolator.h>
#include <vilogger.h>
#include <vinearestneighbournoisedetector.h>
#include <vimahalanobisnoisedetector.h>
#include <vizscorenoisedetector.h>
#include <vimadnoisedetector.h>
#include <vipredictionnoisedetector.h>
#include <vifouriernoisedetector.h>
#include<QTextStream>

#include <iomanip>

#define WINDOW_SIZE 8192
#define NOISE_START 4
#define NOISE_END 16
#define MAX_LENGTH 1000000 // Maximum length of song to process, 10000000 samples is about 2 mins
#define MAX_DEGREE 10

#define MASK_START 0.0
#define MASK_END 0.1
#define MASK_INTERVAL 0.00005
/*#define MASK_END 0.00005
#define MASK_INTERVAL 0.0000001*/



ViBenchMarker::ViBenchMarker()
{
	mInterpolator = NULL;
	mInterpolator2 = NULL;

	mTransformer.setSize(WINDOW_SIZE/2);
	allocateData(WINDOW_SIZE/2);
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
	//benchmark();
	benchmarkNoise();
}

void ViBenchMarker::benchmark(QStringList paths)
{
	clear();
	mFiles = paths;
	mOriginalFiles = mFiles;
	cout << "Benchmarking started (" << paths.size() << " files)" << endl;

	benchmark();
}

void ViBenchMarker::clear()
{
	if(mInterpolator != NULL)
	{
		delete mInterpolator;
		mInterpolator = NULL;
	}
	if(mInterpolator2 != NULL)
	{
		delete mInterpolator2;
		mInterpolator2 = NULL;
	}
	mFile = "";
	mCurrentNoiseSize = 1;
	mInfos.clear();
	mFiles.clear();
	mOriginalFiles.clear();
	mNoises.clear();
	mTotalTime=0;
	mOldProgress=0;
	mTotalProgress = 0;
}

void ViBenchMarker::printProgress(qreal progress)
{
	//progress /= (MASK_END - MASK_START + MASK_INTERVAL) / MASK_INTERVAL;
	if(progress < mOldProgress)
	{
		mTotalProgress += mOldProgress;
		mOldProgress = 0;
	}
	if(progress - mOldProgress > 0.1)
	{
		mOldProgress = progress;
		cout << mTotalProgress + mOldProgress << "%" << endl;
	}
}

void ViBenchMarker::benchmarkNoise()
{
	mCurrentThreshold = MASK_START;

	QObject::connect(&mObjectChain, SIGNAL(progressed(qreal)), this, SLOT(printProgress(qreal)));
	QObject::connect(&mObjectChain, SIGNAL(finished()), this, SLOT(processNoise2()));

	mProject.setFilePath("/home/visore/Visore Projects/XY.vip");
	QObject::connect(&mProject, SIGNAL(finished()), this, SLOT(decodeNoise()));
	mProject.load();
}

void ViBenchMarker::decodeNoise()
{
	QObject::disconnect(&mProject, SIGNAL(finished()), this, SLOT(decodeNoise()));

	mCurrentThreshold = 0.0343; //NN
	//mCurrentThreshold = 0.06885; //Zscore

	mDetector = new ViNearestNeighbourNoiseDetector();
	//mDetector = new ViZscoreNoiseDetector();
	//mDetector = new ViFourierNoiseDetector();
	//mDetector = new ViPredictionNoiseDetector(5);
	//mDetector = new ViMadNoiseDetector();
	//mDetector = new ViMahalanobisNoiseDetector();

	mDetector->clear();
	mDetector->setThreshold(mCurrentThreshold);

	mObjectChain.clear();
	mObjectChain.add(mProject.objects());

	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);
	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted)), 0.04);
	mObjectChain.addFunction(ViFunctionCall("align"), 0.15);
	mObjectChain.addFunction(ViFunctionCall("generateCustomMask"), 0.4);
	mObjectChain.addFunction(ViFunctionCall("generateNoiseMask", QVariant::fromValue(mDetector->clone())), 0.4);
	//mObjectChain.addFunction(ViFunctionCall("encode", QVariant(ViAudio::Custom | ViAudio::CustomMask | ViAudio::Noise | ViAudio::NoiseMask)), 0.04);

	mObjectChain.execute("Generating Noise");
}

void ViBenchMarker::processNoise()
{
	if(mCurrentThreshold > MASK_END)
	{
		qreal truePositives;
		qreal trueNegatives;
		qreal falsePositives;
		qreal falseNegatives;
		for(qreal n = MASK_START; n <= MASK_END; n+= MASK_INTERVAL)
		{
			truePositives = TP.takeFirst();
			trueNegatives = TN.takeFirst();
			falsePositives = FP.takeFirst();
			falseNegatives = FN.takeFirst();

			cout << setprecision(10) << n << "\t" << truePositives << "\t" << trueNegatives << "\t" << falsePositives << "\t" << falseNegatives << "\t" << truePositives / qreal(truePositives + falseNegatives) << "\t" << trueNegatives / qreal(trueNegatives + falsePositives) << endl;
		}
		quit();
	}

	for(int n = 0; n < mProject.objectCount(); ++n)
	{
		int i;
		qint64 truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0;

		cout << "Calculating the noise (threshold: " << mCurrentThreshold << ")" << endl;

		mObject = mProject.object(n);
		ViAudioReadData dataCustom(mObject->buffer(ViAudio::CustomMask));
		dataCustom.setSampleCount(WINDOW_SIZE);
		ViAudioReadData noise(mObject->buffer(ViAudio::Noise));
		noise.setSampleCount(WINDOW_SIZE);

		while(dataCustom.hasData())
		{
			dataCustom.read();
			ViSampleChunk &readCustom1 = dataCustom.splitSamples(0);
			ViSampleChunk &readCustom2 = dataCustom.splitSamples(1);

			noise.read();
			ViSampleChunk &noise1 = noise.splitSamples(0);
			ViSampleChunk &noise2 = noise.splitSamples(1);

			for(i = 0; i < readCustom1.size(); ++i)
			{
				if(readCustom1[i] == 1)
				{
					if(noise1[i] >= mCurrentThreshold) ++truePositives;
					else ++falseNegatives;
				}
				else if(readCustom1[i] == 0)
				{
					if(noise1[i] >= mCurrentThreshold) ++falsePositives;
					else ++trueNegatives;
				}
			}

			for(i = 0; i < readCustom2.size(); ++i)
			{
				if(readCustom2[i] == 1)
				{
					if(noise2[i] >= mCurrentThreshold) ++truePositives;
					else ++falseNegatives;
				}
				else if(readCustom2[i] == 0)
				{
					if(noise2[i] >= mCurrentThreshold) ++falsePositives;
					else ++trueNegatives;
				}
			}
		}

		TP.append(truePositives);
		TN.append(trueNegatives);
		FP.append(falsePositives);
		FN.append(falseNegatives);

		cout << "TP:\t" << truePositives << endl;
		cout << "TN:\t" << trueNegatives << endl;
		cout << "FP:\t" << falsePositives << endl;
		cout << "FN:\t" << falseNegatives << endl;
		cout << "Sensitivity:\t" << setprecision(10) << truePositives / qreal(truePositives + falseNegatives) << endl;
		cout << "Specificity:\t" << setprecision(10) << trueNegatives / qreal(trueNegatives + falsePositives) << endl;
	}

	mCurrentThreshold += MASK_INTERVAL;
	processNoise();
}

void ViBenchMarker::processNoise2()
{
	for(int n = 0; n < mProject.objectCount(); ++n)
	{
		int i, j;
		qint64 truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0;

		QList<qint64> xTP, xFN;
		for(i = 0; i < 1024; ++i)
		{
			xTP.append(0);
			xFN.append(0);
		}

		cout << "Calculating the noise (threshold: " << mCurrentThreshold << ")" << endl;

		mObject = mProject.object(n);
		ViAudioReadData dataCustom(mObject->buffer(ViAudio::CustomMask));
		dataCustom.setSampleCount(WINDOW_SIZE);
		ViAudioReadData noise(mObject->buffer(ViAudio::Noise));
		noise.setSampleCount(WINDOW_SIZE);

		int noiseSize1 = 0, noiseSize2 = 0;;

		while(dataCustom.hasData())
		{
			dataCustom.read();
			ViSampleChunk &readCustom1 = dataCustom.splitSamples(0);
			ViSampleChunk &readCustom2 = dataCustom.splitSamples(1);

			noise.read();
			ViSampleChunk &noise1 = noise.splitSamples(0);
			ViSampleChunk &noise2 = noise.splitSamples(1);

			for(i = 0; i < readCustom1.size(); ++i)
			{
				if(readCustom1[i] == 1)
				{
					if(noiseSize1 == 0)
					{
						for(j = i; j < readCustom1.size(); ++j)
						{
							if(readCustom1[j] == 1) ++noiseSize1;
							else break;
						}
					}

					if(noise1[i] >= mCurrentThreshold)
					{
						xTP[noiseSize1] += 1;
						++truePositives;
					}
					else
					{
						xFN[noiseSize1] += 1;
						++falseNegatives;
					}
				}
				else if(readCustom1[i] == 0)
				{
					noiseSize1 = 0;
					if(noise1[i] >= mCurrentThreshold) ++falsePositives;
					else ++trueNegatives;
				}
			}

			for(i = 0; i < readCustom2.size(); ++i)
			{
				if(readCustom2[i] == 1)
				{
					if(noiseSize2 == 0)
					{
						for(j = i; j < readCustom2.size(); ++j)
						{
							if(readCustom2[j] == 1) ++noiseSize2;
							else break;
						}
					}

					if(noise2[i] >= mCurrentThreshold)
					{
						xTP[noiseSize2] += 1;
						++truePositives;
					}
					else
					{
						xFN[noiseSize2] += 1;
						++falseNegatives;
					}
				}
				else if(readCustom2[i] == 0)
				{
					noiseSize2 = 0;
					if(noise2[i] >= mCurrentThreshold) ++falsePositives;
					else ++trueNegatives;
				}
			}
		}

		TP.append(truePositives);
		TN.append(trueNegatives);
		FP.append(falsePositives);
		FN.append(falseNegatives);
		mTP.append(xTP);
		mFN.append(xFN);

		cout << "TP:\t" << truePositives << endl;
		cout << "TN:\t" << trueNegatives << endl;
		cout << "FP:\t" << falsePositives << endl;
		cout << "FN:\t" << falseNegatives << endl;
		cout << "Sensitivity:\t" << setprecision(10) << truePositives / qreal(truePositives + falseNegatives) << endl;
		cout << "Specificity:\t" << setprecision(10) << trueNegatives / qreal(trueNegatives + falsePositives) << endl;
	}

	qreal truePositives;
	qreal trueNegatives;
	qreal falsePositives;
	qreal falseNegatives;
	for(int n = 0; n < mProject.objectCount(); ++n)
	{
		truePositives = TP.takeFirst();
		trueNegatives = TN.takeFirst();
		falsePositives = FP.takeFirst();
		falseNegatives = FN.takeFirst();

		cout << setprecision(10) << n << "\t" << truePositives << "\t" << trueNegatives << "\t" << falsePositives << "\t" << falseNegatives << "\t" << truePositives / qreal(truePositives + falseNegatives) << "\t" << trueNegatives / qreal(trueNegatives + falsePositives) << "\t";

		QList<qint64> noisesTP = mTP.takeFirst();
		QList<qint64> noisesFN = mFN.takeFirst();
		for(int k = 0; k < noisesTP.size(); ++k)
		{
			if(noisesTP[k] == 0)cout<<"\t"<<0;
			else cout<<"\t"<< noisesTP[k] / qreal(noisesTP[k] + noisesFN[k]);
		}
		cout << endl;
	}
	quit();
}

void ViBenchMarker::benchmark()
{
	//createNoise(mCurrentNoiseSize);


	for(int i = NOISE_START; i <= NOISE_END; ++i)
	{
		mNoises.append(createNoise(i));
		mInfos.append(QList<ViBenchMarkInfo>());
	}

	//mInterpolator = new ViPiecewiseConstantInterpolator();
	//mInterpolator = new ViCosineInterpolator();
	//mInterpolator = new ViPolynomialInterpolator(6);
	//mInterpolator = new ViSplineInterpolator(4);
	//mInterpolator = new ViHermiteInterpolator(6);
	mInterpolator = new ViFourierInterpolator(1);

	//mTime.start();
	processNext();

	/*for(int i = NOISE_START; i <= NOISE_END; ++i)
	{
		mNoises.append(createNoise(i));
	}
	for(int i = 1; i <= MAX_DEGREE; ++i)
	{
		mInfos.append(QList<ViBenchMarkInfo>());
	}
	//mInterpolator2 = new ViPolynomialInterpolator();
	mInterpolator2 = new ViPiecewiseConstantInterpolator();

	mTime.start();
	processNext2();*/
}

void ViBenchMarker::processNext()
{
	if(mFiles.isEmpty())
	{
			for(int j = 0; j < mNoises.size(); ++j)
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

				cout << NOISE_START+j << "\t";
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
		cout << "Time ellapsed: "<<mTotalTime<<" mseconds"<<endl;
		quit();
		return;
	}
	mFile = mFiles.takeFirst();
	cout << "Processing next track: " << mFile.toLatin1().data() << endl;
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
	qreal progress = 0, oldProgress = 0;

	QList<qreal> differences;
	QList<qint64> counts;
	for(int i = 0; i < mNoises.size();++i)
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
		if(currentSamples > MAX_LENGTH) break;

		ViSampleChunk &read1 = data.splitSamples(0);
		ViSampleChunk &read2 = data.splitSamples(1);

		ViSampleChunk write1, write2;
		for(i = 0; i < mNoises.size();++i)
		{
			ViNoise &noise = mNoises[i];

			write1 = data.splitSamples(0);
			write2 = data.splitSamples(1);

			mTime.start();
			mInterpolator->interpolate(write1, noise);
			mInterpolator->interpolate(write2, noise);
			mTotalTime +=mTime.elapsed();

			/*differences[i] += execute(read1, write1);
			differences[i] += execute(read2, write2);
			counts[i] += 2;*/
			//cout<<"++: "<<c.execute(s,s2)<<endl;

			/*for(j = 0; j < noise.size(); ++j)
			{
				if(noise[j])
				{
					differences[i] += qAbs(read1[j] - write1[j]) / 2.0; // Devide by 2 to get the range in [0, 1]
					differences[i] += qAbs(read2[j] - write2[j]) / 2.0;
					counts[i] += 2;
				}
			}*/
		}

		current += (read1.size() + read2.size());
		progress = (current / float(total));
		progress /= mOriginalFiles.size();
		progress += (mOriginalFiles.size() - mFiles.size() - 1) / qreal(mOriginalFiles.size());
		progress *= 100;
		if(progress - oldProgress > 0.05)
		{
			oldProgress = progress;
			cout << "Progress: "<<QString::number(progress, 'f', 2).toLatin1().data()<<"%\t(" <<mTime.elapsed()/1000<<" secs)"  << endl;
		}
	}

	/*QFile file("/home/visore/Desktop/spin2014/RESULTS/"+mInterpolator->name()+".txt");
	file.open(QIODevice::Append);
	QTextStream out(&file);
	out << "\n\n"<<mFile<<"\n";
	for(i = 0; i < mNoises.size();++i)
	{
		ViBenchMarkInfo info;
		info.mFile = mFile;
		info.mDifferences = differences[i];
		info.mCount = counts[i];
		info.mAccuracy = differences[i] / counts[i];//(1 - (differences[i] / (counts[i]  * 2.0)));
		mInfos[i].append(info);
		out<<i+1<<"\t"<<QString::number(info.mAccuracy, 'f', 10)<<"\n";
		//cout<<i+1<<":\t"<<differences[i]<<" "<<counts[i]<<" "<<info.mAccuracy<<endl;
		//cout << "Track accuracy: " << setprecision(10) << info.mAccuracy * 100 << "%" <<endl;
	}
	file.close();*/
	processNext();
}

void ViBenchMarker::processNext2()
{
	if(mFiles.isEmpty())
	{
			for(int j = 0; j < MAX_DEGREE; ++j)
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

				cout << 1+j << "\t";
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
	cout << "Processing next track: " << mFile.toLatin1().data() << endl;
	mBuffer.clear();
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(process2()));
	mCoder.decode(mFile, &mBuffer);
}

void ViBenchMarker::process2()
{
	QObject::disconnect(&mCoder, SIGNAL(finished()), this, SLOT(process2()));

	ViAudioReadData data(&mBuffer);
	data.setSampleCount(WINDOW_SIZE);

	int i, j, k, current = 0, total = MAX_LENGTH; //data.bufferSamples();
	qreal progress = 0, oldProgress = 0;
	qreal degreeavg = 0;
	int degreecount = 0;

	QList<qreal> differences;
	QList<qint64> counts;
	for(int i = 0; i < MAX_DEGREE;++i)
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
		if(currentSamples > MAX_LENGTH) break;


		ViSampleChunk &read1 = data.splitSamples(0);
		ViSampleChunk &read2 = data.splitSamples(1);

		ViSampleChunk write1, write2;
		for(j=0;j<MAX_DEGREE;++j)
		{
			degreeavg = 0;
			degreecount = 0;
			mInterpolator2->setDegree(j+1);

			for(i = 0; i < mNoises.size();++i)
			{
				ViNoise &noise = mNoises[i];



				write1 = data.splitSamples(0);
				write2 = data.splitSamples(1);


				mInterpolator2->interpolate(write1, noise);
				mInterpolator2->interpolate(write2, noise);

				/*degreeavg += execute(read1, write1);
				degreeavg += execute(read2, write2);
				degreecount += 2;*/

				for(k = 0; k < noise.size(); ++k)
				{
					if(noise[k])
					{
						degreeavg += qAbs(read1[k] - write1[k]) / 2.0; // Devide by 2 to get the range in [0, 1]
						degreeavg += qAbs(read2[k] - write2[k]) / 2.0;
						degreecount += 2;
					}
				}
			}
			differences[j] += (degreeavg / degreecount);
			counts[j] += 1;
		}

		current += (read1.size() + read2.size());
		progress = (current / float(total));
		progress /= mOriginalFiles.size();
		progress += (mOriginalFiles.size() - mFiles.size() - 1) / qreal(mOriginalFiles.size());
		progress *= 100;
		if(progress - oldProgress > 0.05)
		{
			oldProgress = progress;
			cout << "Progress: "<<QString::number(progress, 'f', 2).toLatin1().data()<<"%\t(" <<mTime.elapsed()/1000<<" secs)"  << endl;
		}
	}

	QFile file("/home/visore/Desktop/spin2014/RESULTS/"+mInterpolator2->name()+".txt");
	file.open(QIODevice::Append);
	QTextStream out(&file);
	out << "\n\n"<<mFile<<"\n";
	for(i = 0; i < MAX_DEGREE;++i)
	{
		ViBenchMarkInfo info;
		info.mFile = mFile;
		info.mDifferences = differences[i];
		info.mCount = counts[i];
		//info.mAccuracy = differences[i] / counts[i];
		info.mAccuracy = (1 - (differences[i] / (counts[i]  * 2.0)));
		mInfos[i].append(info);
		out<<i+1<<"\t"<<QString::number(info.mAccuracy, 'f', 10)<<"\n";
	}
	file.close();
	processNext2();
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
