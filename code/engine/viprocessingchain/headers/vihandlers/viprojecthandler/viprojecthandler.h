#ifndef VIPROJECTHANDLER_H
#define VIPROJECTHANDLER_H

#include "vihandler.h"

class ViProjectHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void finishOff();
		void finishWriting();
		void finishPlaying();

	public:

		ViProjectHandler(ViProcessingChain *chain);
		void create(ViProject *project, ViAudioFormat format);

	private:

		ViProject *mProject;

};

#endif
