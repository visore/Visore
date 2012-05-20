#ifndef VIFOURIERVARIABLETHREAD_H
#define VIFOURIERVARIABLETHREAD_H

#include <QThread>
#include "FFTReal.h"
#include "vifourierthread.h"

class ViFourierVariableThread : public ViFourierThread
{

	public:

		ViFourierVariableThread();

};

class ViFourierVariableForwardThread : public ViFourierVariableThread
{

	public:

		ViFourierVariableForwardThread();
		void run();

};

class ViFourierVariableInverseThread : public ViFourierVariableThread
{

	public:

		ViFourierVariableInverseThread();
		void run();

};

#endif
