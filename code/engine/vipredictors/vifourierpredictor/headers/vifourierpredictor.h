#ifndef VIFOURIERPREDICTOR_H
#define VIFOURIERPREDICTOR_H

#include <vipredictor.h>

qreal gradient(const qreal &point1, const qreal &point2, const qreal &point3)
{
	return ((point2 - point1) + (point3 - point2)) / 2;
}

class ViFourierPredictor : public ViPredictor
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

		ViFourierPredictor(const Mode &mode = Normal, const Estimation &estimation = Fixed);
		ViFourierPredictor(const ViFourierPredictor &other);
		~ViFourierPredictor();

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);

	protected:

		void setPointers(const Mode &mode, const Estimation &estimation);

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		void predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		void predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);

		/*bool estimateModelNormal(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		void predictModelNormal(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);*/

		bool estimateModelOsculating(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelOsculating(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		/*bool estimateModelSplines(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		void predictModelSplines(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);*/

		void calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative);
		qreal calculateMse(const qreal *observed, const qreal *predicted, const int &size);

	private:

		void (ViFourierPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool (ViFourierPredictor::*estimateModelPointer)(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void (ViFourierPredictor::*predictModelPointer)(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		Estimation mEstimation;
		Mode mMode;

		QVector<int> mBestDegrees;

		int mDegree;
		int mDerivatives;

};

#endif
