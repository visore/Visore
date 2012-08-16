#ifndef VIUNDERRUNHANDLER_H
#define VIUNDERRUNHANDLER_H

#include "vihandler.h"

class ViUnderrunHandler : public ViHandler
{

    Q_OBJECT

	public:

		ViUnderrunHandler(ViProcessingChain *chain);

};

#endif
