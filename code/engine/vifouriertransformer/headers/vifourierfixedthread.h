#ifndef VIFOURIERFIXEDTHREAD_H
#define VIFOURIERFIXEDTHREAD_H

#include <QThread>
#include <QtCore/qmath.h>
#include "FFTRealFixLen.h"

// Each pass of the FFT processes 2^X samples, where X is the number below.
// 4096-point (2^12) FFT object constructed.
static const int FFT_POWER_OF_TWO = 12;
static const int FFT_SAMPLES = qPow(2, FFT_POWER_OF_TWO);

template <typename T>
class ViFourierFixedThread : public QThread
{

	public:

		ViFourierFixedThread();
		void setData(T input[], T output[]);

	protected:

		ffft::FFTRealFixLen<FFT_POWER_OF_TWO> mFourierTransform;
		T *mInput;
		T *mOutput;

};

template <typename T>
class ViFourierFixedForwardThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedForwardThread();
		void run();

};

template <typename T>
class ViFourierFixedInverseThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedInverseThread();
		void run();

};

#include "../sources/vifourierfixedthread.cpp"

#endif
