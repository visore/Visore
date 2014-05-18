#ifndef VIARMAINTERPOLATOR_H
#define VIARMAINTERPOLATOR_H

#include <viinterpolator.h>
#include <vigretl.h>

class ViArmaInterpolator : public ViInterpolator
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

		ViArmaInterpolator();
		~ViArmaInterpolator();

		void setType(const Type &type);
		void setEstimation(const Estimation &estimation);
		void setCriteria(const Criteria &criteria);

		void setWindowSize(const int &size);
		void setDegree(const Type &type, const int &degree);
		void setDegree(const int &arDegree, const int &maDegree);

		void setParameters(const qreal &param1, const qreal &param2, const qreal &param3); // Window size, AR order, MA order

		virtual ViArmaInterpolator* clone();

	protected:

		void initialize();
		void clear();

		virtual bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		MODEL* fixedModel(DATASET *data);
		MODEL* autocorrelationModel(DATASET *data);
		MODEL* bestModel(DATASET *data);
		qreal calculateR2(MODEL *model);
		qreal calculateAR2(MODEL *model);
		qreal calculateAIC(MODEL *model);
		qreal calculateAICC(MODEL *model);
		qreal calculateBIC(MODEL *model);
		qreal calculateHQC(MODEL *model);

	private:

		qreal (ViArmaInterpolator::*orderPointer)(MODEL *model);
		MODEL* (ViArmaInterpolator::*modelPointer)(DATASET *data);

		int *mGretlParameters;
		int mGretlEstimation;

		int mMaDegree, mArDegree;
		Type mType;
		Criteria mCriteria;

};

#endif
