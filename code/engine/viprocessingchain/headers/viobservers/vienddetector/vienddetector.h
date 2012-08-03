#ifndef VIENDDETECTOR_H
#define VIENDDETECTOR_H

#include "viprocessor.h"
#include "viaudioposition.h"

class ViEndDetector : public ViObserver
{

	Q_OBJECT

	signals:

		void songStarted(ViAudioPosition position);
		void songEnded(ViAudioPosition position);
		void recordStarted(ViAudioPosition position);
		void recordEnded(ViAudioPosition position);

	public:

		enum Type
		{
			SongStart = 0,
			SongEnd = 1,
			RecordStart = 2,
			RecordEnd = 3
		};

		ViEndDetector();
		virtual void initialize();

	protected:

		void setRecordStart(ViAudioPosition position);
		void setSongStart(ViAudioPosition position);
		void setRecordEnd(ViAudioPosition position);
		void setSongEnd(ViAudioPosition position);

	protected:

		bool mSongRunning;
		bool mRecordRunning;

};

#endif
