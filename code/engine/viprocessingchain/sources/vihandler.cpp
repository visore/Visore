#include "vihandler.h"

ViHandler::ViHandler(ViProcessingChain *chain)
	: QObject()
{
	mChain = chain;
	mIsEnabled = true;
}

ViProcessingChain* ViHandler::chain()
{
	return mChain;
}

void ViHandler::enable(bool enable)
{
	mIsEnabled = enable;
	if(mIsEnabled)
	{
		blockSignals(true);
	}
	else
	{
		blockSignals(false);
	}
}

void ViHandler::disable()
{
	mIsEnabled = false;
	blockSignals(true);
}

bool ViHandler::isEnabled()
{
	return mIsEnabled;
}

void ViHandler::enableAll(bool enable, bool exceptMe)
{
	for(int i = 0; i < mChain->mHandlers.size(); ++i)
	{
		if(!exceptMe || (exceptMe && mChain->mHandlers[i] != this))
		{
			mChain->mHandlers[i]->enable(enable);
		}
	}
}

void ViHandler::disableAll(bool exceptMe)
{
	for(int i = 0; i < mChain->mHandlers.size(); ++i)
	{
		if(!exceptMe || (exceptMe && mChain->mHandlers[i] != this))
		{
			mChain->mHandlers[i]->disable();
		}
	}
}
