#include "vifourierthread.h"
#include "vifouriertransformer.h"

ViFourierThread::ViFourierThread(ViFourierTransformer *transformer)
	: QThread()
{
	mTransformer = transformer;
	mInput = NULL;
	mOutput = NULL;
	mSize = 0;
}

void ViFourierThread::setData(float input[], float output[])
{
	mInput = input;
	mOutput = output;
}

void ViFourierThread::setSize(int size)
{
	mSize = size;
}

int ViFourierThread::size()
{
	return mSize;
}

void ViFourierThread::notify()
{
	mTransformer->emitFinished();
}
