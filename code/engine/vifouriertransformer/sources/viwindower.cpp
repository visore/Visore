#include "viwindower.h"

ViWindower::ViWindower()
{
	mWindow = NULL;
}

ViWindower::~ViWindower()
{
	if(mWindow != NULL)
	{
		delete mWindow;
	}
}

void ViWindower::apply(float *data, int size)
{
	createWindow(size);
	for(int i = 0; i < size; ++i)
	{
		data[i] *= mWindow->at(i);
	}
}

void ViWindower::createWindow(int size)
{
	if(mWindow == NULL)
	{
		mWindow = new ViWindow(size);
		fillWindow(size);
	}
	else if(size != mWindow->size())
	{
		delete mWindow;
		mWindow = new ViWindow(size);
		fillWindow(size);
	}
}

void ViWindower::fillWindow(int size)
{
	for(int i = 0; i < size; ++i)
	{
		(*mWindow)[i] = calculate(i, size);
	}
}
