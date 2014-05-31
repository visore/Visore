#ifndef VIGARCHINTERPOLATOR_H
#define VIGARCHINTERPOLATOR_H

#include <viinterpolator.h>
#include <vigretl.h>

class ViGarchInterpolator : public ViInterpolator
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

		ViGarchInterpolator(const Estimation &estimation = Fixed);
		ViGarchInterpolator(const ViGarchInterpolator &other);
		~ViGarchInterpolator();

		QString name(QString replace = "", bool spaced = false);

		void setDegree(const int &archDegree, const int &garchDegree);
		void setArchDegree(const int &degree);
		void setGarchDegree(const int &degree);

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		ViGarchInterpolator* clone();

	protected:

		void setEstimation(const Estimation &estimation);

		bool validParameters(const int &windowSize, const int &archDegree, const int &garchDegree);
		bool validParameters(const int &leftSize, const int &rightSize, const int &archDegree, const int &garchDegree);

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

	private:

		Estimation mEstimation;
		ViGretl *mGretl;

};

#endif
