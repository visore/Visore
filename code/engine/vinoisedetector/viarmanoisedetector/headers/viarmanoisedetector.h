/*#ifndef VIARMANOISEDETECTOR_H
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

		enum GretlCriteria
		{
			None,					// Will use fixed order

			AutoCorrelation,		// Uses ACF and PACF
			AC = AutoCorrelation,
			ACF = AutoCorrelation,
			PACF = AutoCorrelation,

			RSquared,				// Only for AR
			AdjustedRSquared,		// Only for AR
			R2 = RSquared,			// Only for AR
			AR2 = AdjustedRSquared,	// Only for AR

			AkaikeInformationCriterion,
			AkaikeInformationCriterionCorrection,
			BayesianInformationCriterion,
			HannanQuinnInformationCriterion,

			AIC = AkaikeInformationCriterion,
			AICC = AkaikeInformationCriterionCorrection,
			BIC = BayesianInformationCriterion,
			HQIC = HannanQuinnInformationCriterion,
			HQC = HannanQuinnInformationCriterion
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
		void setGretlCriteria(const GretlCriteria &criteria);

        void setWindowSize(int size);
		void setDegree(const Type &type, const int &degree);
		void setDegree(const int &arDegree, const int &maDegree);

		QString name(QString replace = "", bool spaced = false);

		void setParameters(const qreal &param1);
		void setParameters(const qreal &param1, const qreal &param2);
		void setParameters(const qreal &param1, const qreal &param2, const qreal &param3);

	protected:

		void calculateNoise(QQueue<qreal> &samples);
		void calculateNoiseNative(QQueue<qreal> &samples);
		void calculateNoiseGretl(QQueue<qreal> &samples);

		//Box-Muller transform - was listed above, and is relatively simple to implement. If you need very precise samples however, be aware that the Box-Muller transform combined with some uniform generators suffers from an anomaly called Neave Effect.
		//H. R. Neave, "On using the Box-Muller transformation with multiplicative congruential pseudorandom number generators," Applied Statistics, 22, 92-97, 1973
		//
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

		void gretlFixedModel(MODEL *model);
		void gretlAutocorrelationModel(MODEL *model);
		void gretlBestModel(MODEL *model);
		qreal gretlR2(MODEL *model);
		qreal gretlAR2(MODEL *model);
		qreal gretlAIC(MODEL *model);
		qreal gretlAICC(MODEL *model);
		qreal gretlBIC(MODEL *model);
		qreal gretlHQC(MODEL *model);

	private:

		void (ViArmaNoiseDetector::*calculateNoisePointer)(QQueue<qreal>&);
		void (ViArmaNoiseDetector::*gretlModelPointer)(MODEL *model);
		qreal (ViArmaNoiseDetector::*gretlOrderPointer)(MODEL *model);

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
		int mGretlEstimation;
		GretlCriteria mGretlCriteria;

		bool mHasNoise;
		qreal mPrediction;
		qreal mPacfConfidenceLevel;

};

#endif

*/
