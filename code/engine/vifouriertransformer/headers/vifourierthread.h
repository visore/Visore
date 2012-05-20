#ifndef VIFOURIERTHREAD_H
#define VIFOURIERTHREAD_H

#include <QThread>

class ViFourierThread : public QThread
{

	public:
		
		ViFourierThread();
		void setData(float input[], float output[]);
		void setSize(int size);
		int size();

		virtual void run() = 0;

	protected:

		float *mInput;
		float *mOutput;
		int mSize;

};

#endif
