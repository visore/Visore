#include <vibenchmarker.h>
#include <viaudiodata.h>
#include <vilinearinterpolator.h>
#include <viadvancedlinearinterpolator.h>
#include <vipiecewiseconstantinterpolator.h>
#include <visplineinterpolator.h>
#include <vilogger.h>

#include <iomanip>
#include <QTime>

#define WINDOW_SIZE 1024

void ViBenchMarker::benchmark(QString path)
{
	mPath = path;
	cout << "Decoding track" <<endl;
	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(process()));
	mCoder.decode(path, &mBuffer);
}

void ViBenchMarker::process()
{
	QObject::disconnect(&mCoder, SIGNAL(finished()), this, SLOT(process()));
	cout << "Benchmarking started" <<endl;

	QList<ViInterpolator*> interpolators = {new ViPiecewiseConstantInterpolator()};
	//QList<ViInterpolator*> interpolators = {new ViLinearInterpolator(), new ViAdvancedLinearInterpolator()};
	//QList<ViInterpolator*> interpolators = {new ViLinearInterpolator()/*, new ViAdvancedLinearInterpolator()*/};
	//QList<ViInterpolator*> interpolators = {/*new ViLinearInterpolator(),*/ new ViAdvancedLinearInterpolator()};
	QList<int> noiseLengths = {2, 4, 8, 12, 16, 24, 32};
	//QList<int> noiseLengths = {8};

	//QList<qreal> LI = {97.23405026, 96.8225023, 96.11406245, 95.02905034, 94.10871158, 93.31781528, 92.58554994, 91.15619808};
	QList<qreal> LI = {	97.23405026,
				96.8225023,
				96.11406245,
				95.02905034,
				94.10871158,
				93.31781528
};


	for(int k = 0; k < noiseLengths.size(); ++k)
	{
		createNoise(noiseLengths[k]);

		ViAudioReadData data(&mBuffer);
		data.setSampleCount(WINDOW_SIZE);

		mBuffer2.clear();
		mBuffer2.setFormat(mBuffer.format());
		ViAudioWriteData data2(&mBuffer2);
		data2.setSampleCount(WINDOW_SIZE);

		QTime time;

		int i, j, oldProgress = 0, newProgress = 0, current = 0, total = data.bufferSamples();

		QList<qint64> times;
		QList<qreal> totalDifferences;
		QList<qint64> differenceCounts;
		for(i = 0; i < interpolators.size(); ++i)
		{
			times.append(0);
			totalDifferences.append(0);
			differenceCounts.append(0);
		}

		while(data.hasData())
		{
			data.read();

			if(data.samples().size() != WINDOW_SIZE) break;

			for(j = 0; j < interpolators.size(); ++j)
			{
				ViSampleChunk &read1 = data.splitSamples(0);
				ViSampleChunk &read2 = data.splitSamples(1);
				ViSampleChunk write1 = data.splitSamples(0);
				ViSampleChunk write2 = data.splitSamples(1);

				time.restart();
				interpolators[j]->interpolate(write1, mNoise);
				interpolators[j]->interpolate(write2, mNoise);
				times[j] += time.elapsed();

				if(j==1)
				{
					data2.enqueueSplitSamples(write1, 0);
					data2.enqueueSplitSamples(write2, 1);
				}

				for(i = 0; i < mNoise.size(); ++i)
				{
					if(mNoise[i])
					{
						totalDifferences[j] += qAbs(read1[i] - write1[i]);
						totalDifferences[j] += qAbs(read2[i] - write2[i]);
						differenceCounts[j] += 2;
					}
				}
			}

			current += (data.splitSamples(0).size() + data.splitSamples(1).size());
			newProgress = (current / float(total)) * 100;
			if(newProgress != oldProgress)
			{
				oldProgress = newProgress;
				//cout << "Progress: " << oldProgress << "%" << endl;
			}
		}

		for(i = 0; i < interpolators.size(); ++i)
		{
			/*cout << "-----------------------------------------" << endl;
			cout << interpolators[i]->name().toLatin1().data() << endl;
			cout << "\tExecution time: " << times[i] << "ms" << endl;
			cout << "\tAccuracy: " << setprecision(10) << (1 - (totalDifferences[i] / (differenceCounts[i] * 2))) * 100 << "%" << endl; // * 2 because samples range from -1 to 1
			cout<<totalDifferences[i] <<"  "<< differenceCounts[i]<<"  "<<totalDifferences[i] /differenceCounts[i]<<endl;*/

			qreal r = (1 - (totalDifferences[i] / (differenceCounts[i] * 2))) * 100;
			//if(r >= LI[k]) cout <<"+\t";
			//else cout <<"-\t";
			cout << fixed<<setprecision(5) << r << "\t";
		}
		cout<<endl;
	}

	QObject::connect(&mCoder, SIGNAL(finished()), this, SLOT(quit()));
	mCoder.encode(&mBuffer2, mPath+"CORRECTED.flac", mBuffer2.format());
	//quit();
}

void ViBenchMarker::createNoise(int length)
{
	int min = 32; // Minimum samples on both sides of noise

	mNoise.resize(WINDOW_SIZE / 2);
	int unit = (min * 2) + length;
	int parts = qFloor(mNoise.size() / float(unit));

	for(int i = 1; i < parts; ++i)
	{
		int start = i * unit;
		int end = start + length;
		for(int j = start; j < end; ++j)
		{if(j>=512)cout<<"****"<<endl;
			mNoise.set(j, 1);
		}
	}

	/*bool b = false;
	for(int i = 0; i < 1024; ++i)
	{
		if(mNoise[i])
		{
			if(!b)
			{
				b = true;
				cout<<i<<" - ";
			}
		}
		else if(b)
		{
			cout<<i-1<<endl;
			b = false;
		}
	}*/
}

void ViBenchMarker::quit()
{
	int x = 0;
	int y = 1 / x;
}
