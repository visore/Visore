#ifndef VIPLAYBACKHANDLER_H
#define VIPLAYBACKHANDLER_H

#include "vihandler.h"

class ViPlaybackHandler : public ViHandler
{

    Q_OBJECT
	
	private slots:

		void handleStart();
		void handleEnd();
		void check();

	public:

		ViPlaybackHandler(ViProcessingChain *chain);

};

#endif
