#ifndef VIPROJECTMANAGER_H
#define VIPROJECTMANAGER_H

#include "viproject.h"

class ViProjectManager : public QObject
{

	Q_OBJECT

	signals:

		void started();
		void finished();
		void statusChanged(QString message);
		void startPlayback();

	public slots:

		void start();
		void startRecord();
		void endRecord();
		void startSong();
		void endSong();

	public:

		ViProjectManager();

		void setProjectName(QString name);
		void setFilePath(QString filePath);
		void setSideCount(short count);
		void setPlayback(bool play);
		
		ViProject* project();
		void reset();

	private:

		void setStatus(QString status);

	private:

		ViProject mProject;
		int mSideCount;
		int mCurrentSide;
		bool mPlayback;

};

#endif
