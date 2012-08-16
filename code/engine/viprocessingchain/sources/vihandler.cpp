#include "vihandler.h"

ViHandler::ViHandler(ViProcessingChain *chain)
	: QObject()
{
	mChain = chain;
}

ViProcessingChain* ViHandler::chain()
{
	return mChain;
}
