#ifndef VIFOURIERVARIABLETHREAD_H
#define VIFOURIERVARIABLETHREAD_H

#include <QThread>
#include "FFTReal.h"
#include "vifourierthread.h"

class ViFourierVariableThread : public ViFourierThread
{

	public:

		ViFourierVariableThread(ViFourierTransformer *transformer);

};

class ViFourierVariableForwardThread : public ViFourierVariableThread
{

	public:

		ViFourierVariableForwardThread(ViFourierTransformer *transformer);
		void run();

};

class ViFourierVariableInverseThread : public ViFourierVariableThread
{

	public:

		ViFourierVariableInverseThread(ViFourierTransformer *transformer);
		void run();

};

class ViFourierVariableRescaleThread : public ViFourierVariableThread
{

	public:

		ViFourierVariableRescaleThread(ViFourierTransformer *transformer);
		void run();

};

#endif
