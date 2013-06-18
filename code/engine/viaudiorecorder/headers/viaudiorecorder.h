#ifndef VIAUDIORECORDER_H
#define VIAUDIORECORDER_H

#include <vinotifier.h>
#include <viproject.h>
#include <vistreaminput.h>
#include <vifrequencysegmentdetector.h>

class ViAudioRecorder : public ViNotifier
{

	Q_OBJECT

	private slots:

		void startRecord();
		void endRecord();
		void startSong();
		void endSong();
		void serialize();
		void finish();
		void checkSize();

	public:

		ViAudioRecorder();
		~ViAudioRecorder();

		bool record(ViProject *project, ViAudio::Type type, ViAudioFormat format, int sides, bool detectInfo);

	protected:

		void nextObject(bool startTimer = true);

	private:

		ViAudioObjectPointer mObject;
		ViAudioObjectQueue mQueue;

		ViStreamInput mInput;
		ViFrequencySegmentDetector mSegmentDetector;

		ViProject *mProject;
		ViAudio::Type mType;

		QTimer mIdleTimer;

		bool mDetectInfo;
        bool mExistingProject;
		int mCurrentTrack;
		int mCurrentSide;
		int mSides;

};

#endif
