#ifndef VIPOLYNOMIALPREDICTOR_H
#define VIPOLYNOMIALPREDICTOR_H

#include <vipredictor.h>

class ViPolynomialPredictor : public ViPredictor
{

	public:

		enum Estimation
		{
			Fixed,
			Best,
		};

		enum Mode
		{
			Normal,
			Osculating,	// Hermite-based: use additional derivatives
			Splines		// Use splines
		};

	public:

		ViPolynomialPredictor(const Mode &mode = Normal, const Estimation &estimation = Fixed);
		ViPolynomialPredictor(const ViPolynomialPredictor &other);
		~ViPolynomialPredictor();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);

		bool validParameters();

	protected:

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);

		void setPointers(const Mode &mode, const Estimation &estimation);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		bool predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);

		bool predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool predictBestDerivative(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);

		bool estimateModelNormal(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelNormal(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		bool estimateModelOsculating(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelOsculating(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		bool estimateModelSplines(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelSplines(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		void calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative, const int &offset, const int multiplier);
		qreal calculateMse(const qreal *observed, const qreal *predicted, const int &size);

	private:

		bool (ViPolynomialPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool (ViPolynomialPredictor::*predictBestPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool (ViPolynomialPredictor::*estimateModelPointer)(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void (ViPolynomialPredictor::*predictModelPointer)(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		Estimation mEstimation;
		Mode mMode;

		QVector<QVector<qint64>> mBestParameters;

		int mDegree;
		int mDerivatives;

};

#endif
