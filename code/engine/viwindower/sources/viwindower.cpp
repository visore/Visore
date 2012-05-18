#include "viwindower.h"

QSharedPointer<ViWindower> ViWindower::mInstance;

ViWindower::ViWindower()
	: ViSingleton()
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

ViWindower* ViWindower::instance()
{
	return NULL;
}

void ViWindower::apply(float *data, qint32 size)
{
	createWindow(size);
	for(int i = 0; i < size; ++i)
	{
		data[i] *= mWindow->at(i);
	}
}

void ViWindower::createWindow(qint32 size)
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

void ViWindower::fillWindow(qint32 size)
{
	for(int i = 0; i < size; ++i)
	{
		(*mWindow)[i] = calculate(i, size);
	}
}
