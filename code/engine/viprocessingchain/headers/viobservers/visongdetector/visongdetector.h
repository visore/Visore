#ifndef VISONGDETECTOR_H
#define VISONGDETECTOR_H

#include "viprocessor.h"
#include "visongidentifier.h"

class ViSongDetector : public ViObserver
{

	Q_OBJECT

	signals:

		void songDetected(ViSongInfo info);

	private slots:

		void setSong(ViSongInfo info);

	public:

		ViSongDetector();
		~ViSongDetector();

		void initialize();
		void finalize();
		void execute();
		void setIdentifier(ViSongIdentifier *identifier);
		ViSongInfo& songInfo();

	private:

		ViSongIdentifier *mIdentifier;
		qint8 mRequestsSent;
		ViSongInfo mSongInfo;

};

#endif
