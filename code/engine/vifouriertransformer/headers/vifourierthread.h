#ifndef VIFOURIERTHREAD_H
#define VIFOURIERTHREAD_H

#include <QThread>

class ViFourierTransformer;

class ViFourierThread : public QThread
{

	public:
		
		ViFourierThread(ViFourierTransformer *transformer);
		void setData(float input[], float output[] = NULL);
		void setSize(int size);
		int size();

		virtual void run() = 0;

	protected:

		void notify();

	protected:

		ViFourierTransformer *mTransformer; // Tight coupling due to template class problems with signals/slots
		float *mInput;
		float *mOutput;
		int mSize;

};

#endif
