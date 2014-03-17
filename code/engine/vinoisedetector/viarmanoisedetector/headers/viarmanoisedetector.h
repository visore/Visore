#ifndef VIARMANOISEDETECTOR_H
#define VIARMANOISEDETECTOR_H

#include <vinoisedetector.h>

class ViArmaNoiseDetector : public ViNoiseDetector
{

    public:

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

		ViArmaNoiseDetector(const Type &type = ARMA);
		ViArmaNoiseDetector(const ViArmaNoiseDetector &other);
        ~ViArmaNoiseDetector();
        ViArmaNoiseDetector* clone();

		void setType(const Type &type);

        void setWindowSize(int size);
		void setDegree(const Type &type, const int &degree);
		void setDegree(const int &maDegree, const int &arDegree);

		QString name(QString replace = "", bool spaced = false);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

		/*
		* Box-Muller transform - was listed above, and is relatively simple to implement. If you need very precise samples however, be aware that the Box-Muller transform combined with some uniform generators suffers from an anomaly called Neave Effect.
		* H. R. Neave, "On using the Box-Muller transformation with multiplicative congruential pseudorandom number generators," Applied Statistics, 22, 92-97, 1973
		 */
		qreal generateNoise(const qreal &variance) const;

		void clear(const Type &type);

		bool leastSquareFit(const qreal *input, const int &degree, qreal *coefficients);
		bool leastSquareFit(const qreal *input, const int &degree, qreal *coefficients, qreal **matrix);
		bool solveEquations(double **matrix, double *coefficients, const int &degree);


	private:

		Type mType;
        int mWindowSize;
        double *mWindowData;

		int mMaDegree, mArDegree;
		qreal **mMaMatrix, **mArMatrix;
		qreal *mMaCoefficients, *mArCoefficients;

};

#endif

