#include "vifourierthread.h"

ViFourierThread::ViFourierThread()
	: QThread()
{
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
