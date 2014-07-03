#ifndef VINEWTONPREDICTOR_H
#define VINEWTONPREDICTOR_H

#include <vipredictor.h>
#include <vieigen.h>

class ViNewtonPredictor : public ViPredictor
{

	public:

		ViNewtonPredictor();
		ViNewtonPredictor(const ViNewtonPredictor &other);
		~ViNewtonPredictor();

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0);

	protected:

		void setType();

		class ViNewtonTypedBase;

	private:

		ViEigenBase *mEigen;
		ViNewtonTypedBase *mTyped;

};

#endif


/*#ifndef VINEWTONPREDICTOR_H
#define VINEWTONPREDICTOR_H

#include <vipredictor.h>

class ViNewtonPredictor : public ViPredictor
{

	public:

		enum Estimation
		{
			Fixed,
			Best,
		};

	public:

		ViNewtonPredictor(const Estimation &estimation = Fixed);
		ViNewtonPredictor(const ViNewtonPredictor &other);
		~ViNewtonPredictor();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);

		void setParameter(const int &number, const qreal &value);

		bool validParameters();

	protected:

		bool validParameters(const int &windowSize, const int &degree);

		void setPointers(const Estimation &estimation);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViErrorCollection *modelErrors = NULL);
		bool predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);

		bool estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		qreal calculateMse(const qreal *observed, const qreal *predicted, const int &size);

	private:

		bool (ViNewtonPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);

		Estimation mEstimation;

		QVector<qint64> mBestParameters;

		int mDegree;

};

#endif
*/
