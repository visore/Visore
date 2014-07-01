#ifndef VIGRETL_H
#define VIGRETL_H

#include <vierror.h>
#include <QtGlobal>
#include <libintl.h> // Very important. Solves the unqualified-id problem in libintl.h. We include it before gretl does
#include <gretl/libgretl.h>
#include <gretl/forecast.h>

// Declared in desribe.c in gretl. Since it is not in describe.h, we manually have to declare it
struct MahalDist_ {
	int *list;
	int n;
	qreal *d;
};

extern "C" int libset_set_int (const char *key, int val); // Not defined as extern in gretl. We have to do it manually
#include <gretl/libset.h>

extern "C" MODEL arma_model (const int *list, const int *pqspec, const DATASET *dset, gretlopt opt, PRN *prn);
extern "C" MODEL garch_model (const int *list, const DATASET *dset, PRN *prn, gretlopt opt);

class ViGretl
{

	public:

		enum Estimation
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

		enum Criteria
		{
			None,					// Will use fixed order

			MeanSquaredError,
			MSE = MeanSquaredError,

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
			Integration,
			AutoRegressiveMovingAverage,
			AutoRegressiveIntegratedMovingAverage,
			AutoRegressiveConditionalHeteroskedasticity,
			GeneralizedAutoRegressiveConditionalHeteroskedasticity,

			MA = MovingAverage,
			AR = AutoRegression,
			I = Integration,
			ARMA = AutoRegressiveMovingAverage,
			ARIMA = AutoRegressiveIntegratedMovingAverage,
			ARCH = AutoRegressiveConditionalHeteroskedasticity,
			GARCH = GeneralizedAutoRegressiveConditionalHeteroskedasticity
		};

	public:

		ViGretl(const Type &type);
		ViGretl(const ViGretl &other);
		~ViGretl();

		static bool initialize(const bool &enableOutput = false);	// Initialize on program start
		static bool uninitialize(); // Uninitialize on program exit

		void adjustWindowSize(int &windowSize);

		void setEstimation(const Estimation &estimation); // Only for ARIMA-based models
		void setCriteria(const Criteria &criteria);
		bool setDegree(const Type &type, const int &degree);
		int degree(const Type &type);

		MODEL* estimate(DATASET *data);
		FITRESID* forecast(DATASET *data, MODEL *model);

		bool forecast(DATASET *data, MODEL *model, qreal *output, const int &size, ViError *error = NULL);
		bool backcast(DATASET *data, MODEL *model, qreal *output, const int &size, ViError *error = NULL);

		bool forecast(const qreal *input, const int &inputSize, qreal *output, const int &outputSize, ViError *error = NULL);
		bool backcast(const qreal *input, const int &inputSize, qreal *output, const int &outputSize, ViError *error = NULL);

		bool interpolate(const qreal *left, const int &leftSize, const qreal *right, const int &rightSize, qreal *output, const int &outputSize, ViError *error = NULL);

		static DATASET* create(const qreal *input, const int &inputSize, const int &outputSize);
		static DATASET* createReversed(const qreal *input, const int &inputSize, const int &outputSize);
		static DATASET* create(const int &inputSize, const int &outputSize);

		static void clear(DATASET *data);
		static void clear(MODEL *model);
		static void clear(FITRESID *cast);

	protected:

		MODEL* fixedArimaModel(DATASET *data);
		MODEL* fixedGarchModel(DATASET *data);

		MODEL* autocorrelationModel(DATASET *data);
		MODEL* bestModel(DATASET *data);

		void estimateArima(MODEL *model, DATASET *data, int *parameters);
		void estimateGarch(MODEL *model, DATASET *data, int *parameters);
		void estimateArch(MODEL *model, DATASET *data, int *parameters);

		qreal calculateMSE(MODEL *model);
		qreal calculateR2(MODEL *model);
		qreal calculateAR2(MODEL *model);
		qreal calculateAIC(MODEL *model);
		qreal calculateAICC(MODEL *model);
		qreal calculateBIC(MODEL *model);
		qreal calculateHQC(MODEL *model);

		int* adjustParameters(int *parameters);
		void adjustSamples(qreal *samples, const int &size);

	private:

		MODEL* (ViGretl::*modelPointer)(DATASET *data);
		MODEL* (ViGretl::*fixedModelPointer)(DATASET *data);
		qreal (ViGretl::*orderPointer)(MODEL *model);
		void (ViGretl::*estimatePointer)(MODEL *model, DATASET *data, int *parameters);

		static bool mInitialized;

		int *mGretlParameters;
		int mGretlEstimation;

		int mMaDegree;
		int mIDegree;
		int mArDegree;
		int mArchDegree;
		int mGarchDegree;

		Type mType;
		Estimation mEstimation;
		Criteria mCriteria;

};

#endif
