#ifndef VIPROJECTHANDLER_H
#define VIPROJECTHANDLER_H

#include "vihandler.h"
#include "viaudioobject.h"
#include "viproject.h"
#include "visongdetector.h"

class ViProjectHandler : public ViHandler
{

    Q_OBJECT

	signals:

		void statusChanged(QString status);
		void progressStarted();
		void progressFinished();

	private slots:

		void addProcessor(ViProcessor *processor);
		void addAudioObject(ViAudioObjectPointer object);
		void finishWriting();

		void changeStatus(QString status);
		void startRecord();
		void endRecord();
		void startSong();
		void endSong();

	public:

		ViProjectHandler(ViProcessingChain *chain);
		~ViProjectHandler();
		void create(QString name, QString filePath, short recordSides);

	private:

		void clearProject();

	private:

		ViProject *mProject;
		ViAudioObjectPointer mObject;

		ViSongDetector *mSongDetector;

};

#endif
