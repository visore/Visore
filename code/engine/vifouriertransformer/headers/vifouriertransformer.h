#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include "vifourierfixedcalculator.h"
#include "vifouriervariablecalculator.h"
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

		void transform(float input[], float output[], Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output);
		void inverseTransform(float input[], float output[]);
		void rescale(float input[]);

		void conjugate(float input[]);
		ViComplexVector toComplex(float input[]);

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
		ViWindowFunction<float> *mWindowFunction;

};

#endif
