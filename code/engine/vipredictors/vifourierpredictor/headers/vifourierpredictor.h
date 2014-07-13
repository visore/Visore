#ifndef VIFOURIERPREDICTOR_H
#define VIFOURIERPREDICTOR_H

#include <vipredictor.h>
#include <vieigen.h>

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

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);

		bool validParameters();

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *modelError = NULL, const int &channel = 0);

	protected:

		void setType();

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);

		void setPointers(const Mode &mode, const Estimation &estimation);

		bool predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError);
		bool predictFixedSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError);

		bool predictBestNormal(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError);
		bool predictBestOsculating(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError);
		bool predictBestSplines(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *modelError);

		ViEigenBaseVector* estimateModelNormal(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelNormal(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets);

		ViEigenBaseVector* estimateModelOsculating(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelOsculating(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets);

		ViEigenBaseVector* estimateModelSplines(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void predictModelSplines(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets);

		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative);
		void calculateDerivative(const int &degree, const qreal &x, ViEigenBaseMatrix *matrix, const int &rowOffset, const int &derivative, const int &offset, const int multiplier);
		qreal calculateMse(const qreal *observed, const qreal *predicted, const int &size);

		void splineOffsetModel(int *offsets, const int &size, const int &coefficientCount);
		void splineOffsetPrediction(int *offsets, const int &predictionCount, const int &size, const int &coefficientCount);

	private:

		bool (ViFourierPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling, ViError *error);
		ViEigenBaseVector* (ViFourierPredictor::*estimateModelPointer)(const int &degree, const int &derivative, const qreal *samples, const int &size, const qreal &scaling);
		void (ViFourierPredictor::*predictModelPointer)(const int &degree, const ViEigenBaseVector *coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling, const int *offsets);

		Estimation mEstimation;
		Mode mMode;

		QVector<QVector<qint64>> mBestParameters;

		int mDegree;
		int mDerivatives;
		ViEigenBase *mEigen;

};

#endif



/*#ifndef VIFOURIERPREDICTOR_H
#define VIFOURIERPREDICTOR_H

#include <vipredictor.h>

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

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &degree);
		void setDerivatives(const int &derivatives);

		void setParameter(const int &number, const qreal &value);

		bool validParameters();

	protected:

		bool validParameters(const Mode &mode, const int &windowSize, const int &degree, const int &derivatives);

		void setPointers(const Mode &mode, const Estimation &estimation);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViErrorCollection *modelErrors = NULL);
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

		bool (ViFourierPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool (ViFourierPredictor::*predictBestPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling);
		bool (ViFourierPredictor::*estimateModelPointer)(const int &degree, const int &derivative, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);
		void (ViFourierPredictor::*predictModelPointer)(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling);

		Estimation mEstimation;
		Mode mMode;

		QVector<QVector<qint64>> mBestParameters;

		int mDegree;
		int mDerivatives;

};

#endif
*/
