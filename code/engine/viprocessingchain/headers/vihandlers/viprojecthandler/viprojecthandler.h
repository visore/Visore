#ifndef VIPROJECTHANDLER_H
#define VIPROJECTHANDLER_H

#include "vihandler.h"

class ViProjectHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void updateProject(ViAudioObjectPointer object);

		void begin();
		void startSong();
		void endSong();
		void startRecord();
		void endRecord();

	public:

		ViProjectHandler(ViProcessingChain *chain);
		void startProject(ViProject *project, ViAudio::Type type, bool existingProject);

	private:

		ViProject *mProject;
		ViAudio::Type mProjectType;

};

#endif
