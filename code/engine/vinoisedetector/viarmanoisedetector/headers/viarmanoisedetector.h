#ifndef VIARMANOISEDETECTOR_H
#define VIARMANOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vigretl.h>

class ViArmaNoiseDetector : public ViNoiseDetector
{

    public:

		enum Mode
		{
			Native,	// Use native code to do the calculations
			Gretl	// Use Gretl code to do the calculations
		};

		enum GretlEstimation
		{
			ExactMaximumLikelihood,
			ExactMaximumLikelihoodLimited,
			ConditionalMaximumLikelihood,
			X12Arma,
			EML = ExactMaximumLikelihood,
			EMLL = ExactMaximumLikelihoodLimited,
			CML = ConditionalMaximumLikelihood,
			X12 = X12Arma
		};

        enum Type
        {
            MovingAverage,
            AutoRegression,
			AutoRegressiveMovingAverage,
			MA = MovingAverage,
			AR = AutoRegression,
			ARMA = AutoRegressiveMovingAverage
        };

    public:

		ViArmaNoiseDetector(const Type &type = ARMA, const Mode &mode = Gretl, const GretlEstimation &estimation = CML);
		ViArmaNoiseDetector(const ViArmaNoiseDetector &other);
        ~ViArmaNoiseDetector();
        ViArmaNoiseDetector* clone();

		void setType(const Type &type);
		void setMode(const Mode &mode = Gretl);
		void setGretlEstimation(const GretlEstimation &estimation);

        void setWindowSize(int size);
		void setDegree(const Type &type, const int &degree);
		void setDegree(const int &maDegree, const int &arDegree);

		QString name(QString replace = "", bool spaced = false);

		void setParameters(qreal param1);
		void setParameters(qreal param1, qreal param2);
		void setParameters(qreal param1, qreal param2, qreal param3);

	protected:

		void calculateNoise(QQueue<qreal> &samples);
		void calculateNoiseNative(QQueue<qreal> &samples);
		void calculateNoiseGretl(QQueue<qreal> &samples);

		/*
		* Box-Muller transform - was listed above, and is relatively simple to implement. If you need very precise samples however, be aware that the Box-Muller transform combined with some uniform generators suffers from an anomaly called Neave Effect.
		* H. R. Neave, "On using the Box-Muller transformation with multiplicative congruential pseudorandom number generators," Applied Statistics, 22, 92-97, 1973
		 */
		//qreal generateNoise(const qreal &variance) const;

		//Marsaglia polar method
		//Use with care. Although also Gaussian noise, the distribution is different to Box-Muller, often give worse results
		qreal generateNoise(const qreal &variance) const;

		void clear(const Type &type = ARMA);

		bool leastSquareFit(const qreal *input, const int &degree, qreal *coefficients);
		bool leastSquareFit(const qreal *input, const int &degree, qreal *coefficients, qreal **matrix);
		bool solveEquations(qreal **matrix, qreal *coefficients, const int &degree);

		qreal mean(const qreal *samples) const;
		qreal mean(const QQueue<qreal> &samples) const;
		qreal variance(const qreal *samples, const qreal &mean) const;
		qreal variance(const QQueue<qreal> &samples, const qreal &mean) const;
		qreal autocorrelation(const int &lag, const qreal *samples, const qreal &mean, const qreal &variance) const;
		void partialAutocorrelation(qreal *partials, const qreal *samples, const qreal &mean, const qreal &variance) const;






		qreal aic(qreal rss, int numberValues, int numberParam)
		{
			//http://adorio-research.org/wordpress/?p=1932
			return 2 * numberParam + numberValues * (log(6.28318530718 * rss/numberValues) + 1);

		}

		qreal aicc(qreal rss, int numberValues, int numberParam)
		{
			//http://adorio-research.org/wordpress/?p=1932
			qreal retval = aic(rss, numberParam, numberValues);
			   if(numberValues-numberParam-1 != 0)
				   retval += 2.0 *numberParam* (numberParam+1)/ qreal(numberValues-numberParam-1);
			   return retval;

		}

		qreal rss(qreal real, qreal pred)
		{
			return qPow(real - pred, 2);
		}

	private:

		void (ViArmaNoiseDetector::*calculateNoisePointer)(QQueue<qreal>&);

		Mode mMode;
		Type mType;
        int mWindowSize;
		qreal *mWindowData;

		int mMaDegree, mArDegree;
		qreal **mMaMatrix, **mArMatrix;
		qreal *mMaCoefficients, *mArCoefficients;
		qreal **mPartialMatrix;

		DATASET *mGretlData;
		int *mGretlParameters;
		MODEL *mGretlModel;
		int mGretlEstimation;
		QVector<qreal> mGretlPredictions;


};

#endif

