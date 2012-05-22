#ifndef VIFOURIERFIXEDTHREAD_H
#define VIFOURIERFIXEDTHREAD_H

#include <qmath.h> 
#include "FFTRealFixLen.h"
#include "vifourierthread.h"

template <int T>
class ViFourierFixedThread : public ViFourierThread
{

	public:
		
		ViFourierFixedThread(ViFourierTransformer *transformer);
		virtual void run() = 0;

	protected:

		ffft::FFTRealFixLen<T> mFourierTransform;

};

template <int T>
class ViFourierFixedForwardThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedForwardThread(ViFourierTransformer *transformer);
		void run();

};

template <int T>
class ViFourierFixedInverseThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedInverseThread(ViFourierTransformer *transformer);
		void run();

};

template <int T>
class ViFourierFixedRescaleThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedRescaleThread(ViFourierTransformer *transformer);
		void run();

};

#include "../sources/vifourierfixedthread.cpp"

#endif
