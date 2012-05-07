#ifndef VIFOURIERVARIABLETHREAD_H
#define VIFOURIERVARIABLETHREAD_H

#include <QThread>
#include "FFTReal.h"

template <typename T>
class ViFourierVariablehread : public QThread
{

	public:

		ViFourierVariablehread();
		void setData(T input[], T output[], int numberOfSamples);

	protected:

		T *mInput;
		T *mOutput;
		int mNumberOfSamples;

};

template <typename T>
class ViFourierVariableForwardThread : public ViFourierVariablehread<T>
{

	public:

		ViFourierVariableForwardThread();
		void run();

};

template <typename T>
class ViFourierVariableInverseThread : public ViFourierVariablehread<T>
{

	public:

		ViFourierVariableInverseThread();
		void run();

};

#include "../sources/vifouriervariablethread.cpp"

#endif
