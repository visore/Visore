#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include "vifouriercalculator.h"
#include "viwindowfunction.h"
#include "vicomplexnumber.h"
#include <QMap>

typedef QVector<ViComplexFloat> ViComplexVector;

class ViFourierTransformer
{

	public:

		enum Direction
		{
			Forward = 0,
			Inverse = 1
		};

		enum Initialization
		{
			VariableSize = 0,
			FixedSize = 1,
			InvalidSize = 2
		};

	public:

		ViFourierTransformer(int size = 0, QString functionName = "");
		~ViFourierTransformer();

		Initialization setSize(int size);
		bool setWindowFunction(QString functionName);
		QStringList windowFunctions();

		void transform(double input[], double output[], Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(double input[], double output[]);
		void inverseTransform(double input[], double output[]);
		void rescale(double input[]);

		void conjugate(double input[]);
		void pad(double input[], int numberOfSamples, double value = 0);
		void multiply(double first[], double second[], double output[]);
		ViComplexVector toComplex(double input[]);

	protected:

		void initialize();
		int sizeToKey(int size);
		bool isValidSize(int value);

	private:

		int mSize;
		QMap<int, ViFourierCalculator*> mFixedCalculators;
		ViFourierCalculator* mVariableCalculator;
		ViFourierCalculator *mCalculator;
		QStringList mWindowFunctions;
		ViWindowFunction<double> *mWindowFunction;

};

#endif
