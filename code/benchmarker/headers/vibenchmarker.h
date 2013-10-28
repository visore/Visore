#ifndef VIBENCHMARKER_H
#define VIBENCHMARKER_H

#include <vibuffer.h>
#include <vinoise.h>
#include <viaudiocoder.h>
#include <viinterpolator.h>
#include <QTime>
#include<vicrosscorrelator.h>
#include <viproject.h>

class ViBenchMarker : public QObject
{

	Q_OBJECT

	private slots:

		void processNext();
		void process();

		void processNext2();
		void process2();

		void processNoise();
		void decodeNoise1();
		void decodeNoise2();
		void decodeNoise3();

		void quit();

	public:

		ViBenchMarker();
		virtual ~ViBenchMarker();

		void benchmark(QString path);
		void benchmark(QStringList paths);

	private:

		void clear();
		void benchmark();
		void benchmarkNoise();
		ViNoise createNoise(int length);

	private:

		struct ViBenchMarkInfo
		{
			QString mFile;
			qreal mAccuracy;
			qreal mDifferences;
			qint64 mCount;
		};

		ViBuffer mBuffer;
		ViBuffer mBuffer2;
		ViAudioCoder mCoder;

		QString mFile;
		QStringList mFiles;
		QStringList mOriginalFiles;
		ViInterpolator *mInterpolator;
		ViDegreeInterpolator *mInterpolator2;
		ViNoise mNoise;
		QList< QList<ViBenchMarkInfo> > mInfos;

		QList<ViNoise> mNoises;

		int mCurrentNoiseSize;
		int mCurrentDegree;

		QTime mTime;

		qint64 mTotalTime;

		ViNoiseDetector *mDetector;

		ViProject mProject;
		ViAudioObjectPointer mObject;
















		ViFourierTransformer mTransformer;
		qreal *mRealData;
		qreal *mFirstFourierData;
		qreal *mSecondFourierData;
		qreal *mMultiplyData;
		qreal *mAutocorrelationData;

		qreal execute(const ViSampleChunk &theSamples1, const ViSampleChunk &theSamples2)
		{
			qreal firstNorm, secondNorm, multipliedNorm;
			int count = theSamples1.size();

			memcpy(mRealData, theSamples1.data(), theSamples1.size() * sizeof(qreal));
			firstNorm = norm(mRealData, theSamples1.size());
			multipliedNorm = firstNorm * firstNorm;
			mTransformer.pad(mRealData, theSamples1.size());
			mTransformer.forwardTransform(mRealData, mFirstFourierData);

			//Auto-correlation
			memcpy(mRealData, mFirstFourierData, count * sizeof(qreal));
			mTransformer.conjugate(mFirstFourierData);
			mTransformer.multiply(mFirstFourierData, mRealData, mMultiplyData);
			mTransformer.inverseTransform(mMultiplyData, mAutocorrelationData);
			mTransformer.rescale(mAutocorrelationData);
			applyNorm(mAutocorrelationData, count, multipliedNorm);

			//Cross-correlation
			memcpy(mRealData, theSamples2.data(), theSamples2.size() * sizeof(qreal));
			secondNorm = norm(mRealData, theSamples2.size());
			multipliedNorm = firstNorm * secondNorm;
			mTransformer.pad(mRealData, theSamples2.size());
			mTransformer.forwardTransform(mRealData, mSecondFourierData);
			mTransformer.multiply(mFirstFourierData, mSecondFourierData, mMultiplyData);
			mTransformer.inverseTransform(mMultiplyData, mRealData);
			mTransformer.rescale(mRealData);
			applyNorm(mRealData, count, multipliedNorm);

			qreal result = 0;
			for(int i = 0; i < count; ++i)
			{
			   result += (1 - (qAbs(mRealData[i] - mAutocorrelationData[i]) / 2));
			}
			return result / count;
		}

		void allocateData(int samples)
		{
			mRealData = new qreal[samples];
			mFirstFourierData = new qreal[samples];
			mSecondFourierData = new qreal[samples];
			mMultiplyData = new qreal[samples];
			mAutocorrelationData = new qreal[samples];
		}

		void deallocateData()
		{
			if(mRealData != NULL)
			{
				delete [] mRealData;
				mRealData = NULL;
			}
			if(mFirstFourierData != NULL)
			{
				delete [] mFirstFourierData;
				mFirstFourierData = NULL;
			}
			if(mSecondFourierData != NULL)
			{
				delete [] mSecondFourierData;
				mSecondFourierData = NULL;
			}
			if(mMultiplyData != NULL)
			{
				delete [] mMultiplyData;
				mMultiplyData = NULL;
			}
			if(mAutocorrelationData != NULL)
			{
				delete [] mAutocorrelationData;
				mAutocorrelationData = NULL;
			}
		}

		qreal norm(qreal array[], qint32 size)
		{
			qreal norm = 0;
			for(int i = 0; i < size; ++i)
			{
				norm += (array[i] * array[i]);
			}
			return qSqrt(norm);
		}

		void applyNorm(qreal array[], qint32 size, qreal norm)
		{
			if(norm != 0)
			{
				for(int i = 0; i < size; ++i)
				{
					array[i] /= norm;
				}
			}
		}
















};

#endif
