#ifndef VIPROJECTHANDLER_H
#define VIPROJECTHANDLER_H

#include "vihandler.h"
#include "viaudioobject.h"

class ViProjectHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void addAudioObject(ViAudioObject *object);
		void finishWriting();
		void finishPlaying();

	public:

		ViProjectHandler(ViProcessingChain *chain);
		void create(ViProject *project, ViAudioFormat format);

	private:

		ViProject *mProject;

};

#endif
