#ifndef VIARIMAINTERPOLATOR_H
#define VIARIMAINTERPOLATOR_H

#include <viinterpolator.h>
#include <vigretl.h>

class ViArimaInterpolator : public ViInterpolator
{

	public:

		enum Estimation
		{
			Fixed,

			MSE,
			AC,
			AIC,
			AICC,
			BIC,
			HQC
		};

	public:

		ViArimaInterpolator(const Estimation &estimation = Fixed);
		ViArimaInterpolator(const ViArimaInterpolator &other);
		~ViArimaInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &arDegree, const int &iDegree, const int &maDegree);
		void setArDegree(const int &degree);
		void setIDegree(const int &degree);
		void setMaDegree(const int &degree);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViArimaInterpolator* clone();

	protected:

		void setEstimation(const Estimation &estimation);

		bool validParameters(const int &windowSize, const int &arDegree, const int &iDegree, const int &maDegree);
		bool validParameters(const int &leftSize, const int &rightSize, const int &arDegree, const int &iDegree, const int &maDegree);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel);

	private:

		Estimation mEstimation;
		ViGretl *mGretl;

};

#endif
