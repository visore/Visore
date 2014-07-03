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

		ViPredictionNoiseDetector(ViPredictor *predictor); // Takes ownership
		~ViPredictionNoiseDetector();

		QString name(QString replace = "", bool spaced = false);

		void setThreshold(const qreal threshold);

		bool validParameters();

	protected:

		void initialize(const int &channelCount);
		void detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);

	private:

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
		int mCounter;

};

#endif

