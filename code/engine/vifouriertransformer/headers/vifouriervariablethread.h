#ifndef VIFOURIERVARIABLETHREAD_H
#define VIFOURIERVARIABLETHREAD_H

#include <QThread>
#include "FFTReal.h"

class ViFourierVariablehread : public QThread
{

	public:

		ViFourierVariablehread();
		void setData(float input[], float output[], int numberOfSamples);

	protected:

		float *mInput;
		float *mOutput;
		int mNumberOfSamples;

};

class ViFourierVariableForwardThread : public ViFourierVariablehread
{

	public:

		ViFourierVariableForwardThread();
		void run();

};

class ViFourierVariableInverseThread : public ViFourierVariablehread
{

	public:

		ViFourierVariableInverseThread();
		void run();

};

#endif
