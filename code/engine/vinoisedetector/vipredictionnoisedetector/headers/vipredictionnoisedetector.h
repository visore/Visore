#ifndef VIPREDICTIONNOISEDETECTOR_H
#define VIPREDICTIONNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vipredictor.h>

class ViPredictionNoiseDetector : public ViNoiseDetector
{

	Q_OBJECT

	protected slots:

		void changeParameter(QString name, qreal value);

	public:

		enum PredictionMode
		{
			Batch,
			Recurrent
		};

    public:

		ViPredictionNoiseDetector(ViPredictor *predictor, PredictionMode mode = Recurrent); // Takes ownership
		~ViPredictionNoiseDetector();

		QString name(QString replace = "", bool spaced = false);

		void setThreshold(const qreal threshold);
		void setPredictionMode(PredictionMode mode);

		bool validParameters();

	protected:

		void initialize(const int &channelCount);
		void detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);

		void detectBatch(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);
		void detectRecurrent(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);

	private:

		PredictionMode mPredictionMode;
		int mWindowSize;
		int mRequiredSize;
		ViPredictor *mPredictor;
		qreal mPrediction;
		qreal *mPredictions;
		int mPredictionCount;
		qreal mDifference;
		qreal mThreshold;
		int mI;
		int mJ;
		qreal mMean;
		qreal mMaxNoise;
		int mCounter;

		QVector<int> mNoiseCounter;

};

#endif

