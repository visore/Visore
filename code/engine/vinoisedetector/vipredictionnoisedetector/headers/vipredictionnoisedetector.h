#ifndef VIPREDICTIONNOISEDETECTOR_H
#define VIPREDICTIONNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vipredictor.h>

class ViPredictionNoiseDetector : public ViNoiseDetector
{

	Q_OBJECT

	private slots:

		void changeParameter(QString name, qreal value);

    public:

		ViPredictionNoiseDetector(ViPredictor *predictor); // Takes ownership
		~ViPredictionNoiseDetector();

		QString name(QString replace = "", bool spaced = false);

		bool validParameters();

	protected:

		void initialize();
		void detect(QVector<qreal> &samples, QVector<qreal> &noise);

	private:

		int mWindowSize;
		int mRequiredSize;
		ViPredictor *mPredictor;
		qreal mPrediction;
		qreal *mPredictions;
		int mPredictionCount;
		qreal mDifference;

};

#endif

