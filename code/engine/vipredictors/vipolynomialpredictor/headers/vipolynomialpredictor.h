#ifndef VIPOLYNOMIALPREDICTOR_H
#define VIPOLYNOMIALPREDICTOR_H

#include <vipredictor.h>
#include <vieigen.h>

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

		void setType();

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);

		void setPointers(const Mode &mode, const Estimation &estimation);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *modelError = NULL);
		bool predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError = NULL);
		bool predictFixedSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError = NULL);

		bool predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError = NULL);
		bool predictBestOsculating(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError = NULL);
		bool predictBestSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError = NULL);

		ViEigenBaseVector* estimateModelNormal(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets = NULL);

		ViEigenBaseVector* estimateModelOsculating(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets = NULL);

		ViEigenBaseVector* estimateModelSplines(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets = NULL);

		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier);
		qreal calculateMse(const qreal *observed, const qreal *predicted, const int &size);

		void splineOffsetModel(int *offsets, const int &size, const int &coefficientCount);
		void splineOffsetPrediction(int *offsets, const int &predictionCount, const int &size, const int &coefficientCount);

	private:

		bool (ViPolynomialPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error = NULL);
		ViEigenBaseVector* (ViPolynomialPredictor::*estimateModelPointer)(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void (ViPolynomialPredictor::*predictModelPointer)(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets = NULL);

		Estimation mEstimation;
		Mode mMode;

		QVector<QVector<qint64>> mBestParameters;

		int mDegree;
		int mDerivatives;
		ViEigenBase *mEigen;

};

#endif
