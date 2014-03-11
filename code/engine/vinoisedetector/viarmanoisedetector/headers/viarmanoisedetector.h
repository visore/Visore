#ifndef VIARMANOISEDETECTOR_H
#define VIARMANOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vimatrix.h>

class CArma;

class ViArmaNoiseDetector : public ViNoiseDetector
{

    public:

        enum Type
        {
            MovingAverage,
            AutoRegression,
            AutoRegressiveMovingAverage
        };

    public:

        ViArmaNoiseDetector(const int &degree = 2);
        ~ViArmaNoiseDetector();
        ViArmaNoiseDetector* clone();

      //  void setType(const Type &type);

        void setWindowSize(int size);
		void setDegree(const int &degree);
		int degree();

		QString name(QString replace = "", bool spaced = false);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

        int mWindowSize;
		int mDegree;
        Type mType;
        CArma *mArma;
        double *mWindowData;

};

#endif

