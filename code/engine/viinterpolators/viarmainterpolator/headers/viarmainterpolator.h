#ifndef VIARMAINTERPOLATOR_H
#define VIARMAINTERPOLATOR_H

#include <viinterpolator.h>
#include <vigretl.h>

class ViArmaInterpolator : public ViAutoDegreeInterpolator
{

	public:

		ViArmaInterpolator();
		ViArmaInterpolator(const int &degree);
		~ViArmaInterpolator();

		virtual ViArmaInterpolator* clone();

	protected:

		void initialize();
		void clear();

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		void gretlFixedModel(MODEL *model);
		void gretlAutocorrelationModel(MODEL *model);

	private:

		DATASET *mGretlData;
		int *mGretlParameters;
		int mWindowSize;
		qreal mPacfConfidenceLevel;

};

#endif
