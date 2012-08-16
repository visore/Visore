#ifndef VIHANDLER_H
#define VIHANDLER_H

#include "viprocessingchain.h"

class ViHandler : public QObject
{

    Q_OBJECT

	public:

		ViHandler(ViProcessingChain *chain);
		ViProcessingChain* chain();

	protected:

		ViProcessingChain *mChain;

};

#endif
