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

#define MASK_START 0
#define MASK_END 0.3
//#define MASK_INTERVAL 0.0001
#define MASK_INTERVAL 0.002

/*#define MASK_END 0.00005
#define MASK_INTERVAL 0.0000001*/



ViBenchMarker::ViBenchMarker()
{
	mDetector=NULL;
}

ViBenchMarker::~ViBenchMarker()
{
	clear();
}

void ViBenchMarker::benchmark()
{
	clear();
	QString d = "/home/visore/Visore Projects/NOISE/";
	QDir dir(d);
	mProjects.append(dir.entryList(QDir::Files));
	//mProjects.append("a.vip");
	for(int i=0;i<mProjects.size();++i)mProjects[i]=d+mProjects[i];

	for(qreal q = MASK_START; q <= MASK_END; q += MASK_INTERVAL)
	{
		TP[q] = 0;
		TN[q] = 0;
		FP[q] = 0;
		FN[q] = 0;
	}

	//mCurrentThreshold = 0.0343; //NN
	//mCurrentThreshold = 0.06885; //Zscore

	mDetector = new ViNearestNeighbourNoiseDetector();
	//mDetector = new ViZscoreNoiseDetector();
	//mDetector = new ViFourierNoiseDetector();
	//mDetector = new ViPredictionNoiseDetector(5);
	//mDetector = new ViMadNoiseDetector();
	//mDetector = new ViMahalanobisNoiseDetector();

	QObject::connect(&mObjectChain, SIGNAL(progressed(qreal)), this, SLOT(printProgress(qreal)), Qt::UniqueConnection);
	QObject::connect(&mObjectChain, SIGNAL(finished()), this, SLOT(process()), Qt::UniqueConnection);

	loadProject(mProjects.dequeue());
}

void ViBenchMarker::clear()
{
	mCurrentNoiseSize = 1;
	mNoises.clear();
	mTotalTime=0;
	mOldProgress=0;
	mTotalProgress = 0;
}

void ViBenchMarker::loadProject(QString path)
{
	mCurrentThreshold = MASK_START;
	mObjectChain.clear();
	mProject.clear();
	//mProject = ViProject();
	mProject.setFilePath(path);
	cout<<endl<<mProject.fileName().toLatin1().data()<<endl;
	QObject::connect(&mProject, SIGNAL(finished()), this, SLOT(processProject()));
	mProject.load();
}

void ViBenchMarker::printProgress(qreal progress)
{
	//progress /= (MASK_END - MASK_START + MASK_INTERVAL) / MASK_INTERVAL;
	/*if(progress < mOldProgress)
	{
		mTotalProgress += mOldProgress;
		mOldProgress = 0;
	}
	if(progress - mOldProgress > 0.1)
	{
		mOldProgress = progress;
		cout << mTotalProgress + mOldProgress << "%" << endl;
	}*/
}

void ViBenchMarker::processProject()
{
	QObject::disconnect(&mProject, SIGNAL(finished()), this, SLOT(processProject()));

	mDetector->clear();
	mDetector->setThreshold(mCurrentThreshold);

	mObjectChain.clear();
	mObjectChain.add(mProject.objects());

	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted)), 0.1);
	mObjectChain.addFunction(ViFunctionCall("align"), 0.2);
	mObjectChain.addFunction(ViFunctionCall("generateCustomMask"), 0.3);
	mObjectChain.addFunction(ViFunctionCall("generateNoiseMask", QVariant::fromValue(mDetector)), 0.3);
	mObjectChain.addFunction(ViFunctionCall("clearBuffers", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted)), 0.1);

	mObjectChain.execute("Generating Noise");
}

void ViBenchMarker::process()
{
	processTuning();
}

void ViBenchMarker::processTuning()
{
	int i, n;
	for(n = 0; n < mProject.objectCount(); ++n)
	{
		mObject = mProject.object(n);
		cout << mObject->fileName().toLatin1().data() << endl;

		ViAudioReadData mask(mObject->buffer(ViAudio::Noise));
		mask.setSampleCount(WINDOW_SIZE);
		ViSampleChunk *nData1 = new ViSampleChunk(mask.bufferSamples());
		ViSampleChunk *nData2 = new ViSampleChunk(mask.bufferSamples());
		ViSampleChunk *nMask1 = new ViSampleChunk(mask.bufferSamples());
		ViSampleChunk *nMask2 = new ViSampleChunk(mask.bufferSamples());

		qint offset1 = 0, offset2 = 0;
		while(mask.hasData())
		{
			mask.read();
			ViSampleChunk &mask1 = mask.splitSamples(0);
			ViSampleChunk &mask2 = mask.splitSamples(1);

			for(i = 0; i < mask1.size(); ++i) (*nData1)[i + offset1] = mask1[i];
			offset1 += mask1.size();
			for(i = 0; i < mask2.size(); ++i) (*nData2)[i + offset2] = mask2[i];
			offset2 += mask2.size();
		}

		ViNoise noise1(nData1, nMask1, mCurrentThreshold);
		ViNoise noise2(nData2, nMask2, mCurrentThreshold);

		for(mCurrentThreshold = MASK_START; mCurrentThreshold <= MASK_END; mCurrentThreshold += MASK_INTERVAL)
		{
			noise1.setThreshold(mCurrentThreshold);
			noise1.generateMask(ViNoise::Direct);
			noise2.setThreshold(mCurrentThreshold);
			noise2.generateMask(ViNoise::Direct);

			ViAudioReadData custom(mObject->buffer(ViAudio::CustomMask));
			custom.setSampleCount(WINDOW_SIZE);

			qint64 truePositives = 0, falsePositives = 0, trueNegatives = 0, falseNegatives = 0;
			offset1 = 0, offset2 = 0;

			while(custom.hasData())
			{
				custom.read();
				ViSampleChunk &custom1 = custom.splitSamples(0);
				ViSampleChunk &custom2 = custom.splitSamples(1);

				for(i = 0; i < custom1.size(); ++i)
				{
					if(custom1[i] == 1)
					{
						if((*nMask1)[i+offset1] == 1) ++truePositives;
						else ++falseNegatives;
					}
					else if(custom1[i] == 0)
					{
						if((*nMask1)[i+offset1] == 1) ++falsePositives;
						else ++trueNegatives;
					}
				}
				offset1+=custom1.size();

				for(i = 0; i < custom2.size(); ++i)
				{
					if(custom2[i] == 1)
					{
						if((*nMask2)[i+offset2] == 1) ++truePositives;
						else ++falseNegatives;
					}
					else if(custom2[i] == 0)
					{
						if((*nMask2)[i+offset2] == 1) ++falsePositives;
						else ++trueNegatives;
					}
				}
				offset2+=custom2.size();
			}

			cout << "\t" << mCurrentThreshold << "\t" << truePositives << "\t" << trueNegatives << "\t" << falsePositives << "\t" << falseNegatives << "\t";
			cout << setprecision(10) << truePositives / qreal(truePositives + falseNegatives) << "\t";
			cout << setprecision(10) << trueNegatives / qreal(trueNegatives + falsePositives) << endl;

			TP[mCurrentThreshold] += truePositives;
			TN[mCurrentThreshold] += trueNegatives;
			FP[mCurrentThreshold] += falsePositives;
			FN[mCurrentThreshold] += falseNegatives;
		}
		mObject->clearBuffers();
	}

	if(mProjects.isEmpty())
	{
		cout << "THRESHOLD\tTP\tTN\tFP\tFN\tSEN\tSPE" << endl;
		foreach(qreal k, TP.keys())
		{
			cout << k << "\t" << TP[k] << "\t" << TN[k] << "\t" << FP[k] << "\t" << FN[k];
			cout << "\t" << setprecision(10) << TP[k] / qreal(TP[k] + FN[k]);
			cout << "\t" << setprecision(10) <<TN[k] / qreal(TN[k] + FP[k]) << endl;
		}
		//cout << endl << endl << "FINISHED!!"<<endl;
		quit();
	}
	loadProject(mProjects.dequeue());
}

/*void ViBenchMarker::processNoise2()
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
}*/

void ViBenchMarker::quit()
{
	int x = 0;
	int y = 1 / x;
}
