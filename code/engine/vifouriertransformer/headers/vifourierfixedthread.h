#ifndef VIFOURIERFIXEDTHREAD_H
#define VIFOURIERFIXEDTHREAD_H

#include <QThread>
#include <QtCore/qmath.h>
#include "FFTRealFixLen.h"

// Each pass of the FFfloat processes 2^X samples, where X is the number below.
// 4096-point (2^12) FFfloat object constructed.
static const int FFT_POWER_OF_TWO = 12;
static const int FFT_SAMPLES = qPow(2, FFT_POWER_OF_TWO);

class ViFourierFixedThread : public QThread
{

	public:
		
		ViFourierFixedThread();
		void setData(float input[], float output[]);

	protected:

		ffft::FFTRealFixLen<FFT_POWER_OF_TWO> mFourierTransform;
		float *mInput;
		float *mOutput;

};

class ViFourierFixedForwardThread : public ViFourierFixedThread
{

	public:

		ViFourierFixedForwardThread();
		void run();

};

class ViFourierFixedInverseThread : public ViFourierFixedThread
{

	public:

		ViFourierFixedInverseThread();
		void run();

};

#endif
