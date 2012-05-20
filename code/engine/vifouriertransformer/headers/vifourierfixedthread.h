#ifndef VIFOURIERFIXEDTHREAD_H
#define VIFOURIERFIXEDTHREAD_H

#include <QtCore/qmath.h> 
#include "FFTRealFixLen.h"
#include "vifourierthread.h"

template <int T>
class ViFourierFixedThread : public ViFourierThread
{

	public:
		
		ViFourierFixedThread();
		virtual void run() = 0;

	protected:

		ffft::FFTRealFixLen<T> mFourierTransform;

};

template <int T>
class ViFourierFixedForwardThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedForwardThread();
		void run();

};

template <int T>
class ViFourierFixedInverseThread : public ViFourierFixedThread<T>
{

	public:

		ViFourierFixedInverseThread();
		void run();

};

#include "../sources/vifourierfixedthread.cpp"

#endif
