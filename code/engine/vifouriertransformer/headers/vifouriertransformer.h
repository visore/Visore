#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include "vifouriercalculator.h"
#include "viwindowfunction.h"
#include "vicomplexnumber.h"
#include <QMap>
#include <viid.h>

typedef QVector<ViDoubleComplex> ViComplexVector;

class ViFourierTransformer : public ViId
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

		ViFourierTransformer(const int size = 0, const QString functionName = "");
		virtual ~ViFourierTransformer();

		Initialization setSize(const int size);
		bool setWindowFunction(const QString functionName);
		QStringList windowFunctions() const;

		void transform(const double input[], double output[], const Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(const double input[], double output[]);
		void inverseTransform(const double input[], double output[]);
		void rescale(double input[]);

		void conjugate(double input[]) const;
		void pad(double input[], const int numberOfSamples, const double value = 0) const;
		void multiply(const double first[], const double second[], double output[]) const;
		ViComplexVector toComplex(const double input[]) const;

	protected:

		void initialize();
		int sizeToKey(const int size) const;
		bool isValidSize(const int value) const;

	private:

		int mSize;
		double *mWindowData;
		QMap<int, ViFourierCalculator*> mFixedCalculators;
		ViFourierCalculator* mVariableCalculator;
		ViFourierCalculator *mCalculator;
		QStringList mWindowFunctions;
		ViWindowFunction<double> *mWindowFunction;

};

#endif
