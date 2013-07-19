#ifndef VIAUDIOPLAYER_H
#define VIAUDIOPLAYER_H

#include <viaudioobject.h>
#include <vistreamoutput.h>

class ViAudioPlayer : public QObject
{

	Q_OBJECT

	signals:

		void played();
		void paused();
		void stopped();
		void positionChanged(ViAudioPosition position);
		void durationChanged(ViAudioPosition duration);
		void volumeChanged(int volume);

	public slots:

		void play();
		void pause();
		void stop();
		void changePosition(ViAudioPosition position);
		void changeVolume(int volume);

	private slots:

		void checkUnderrun();
		void checkDuration();

	public:

		ViAudioPlayer();
		~ViAudioPlayer();

		void clear();

		void setData(ViAudioObjectPointer object, ViAudio::Type type, bool startPlaying = false);
		void setData(ViBuffer *buffer, bool startPlaying = false);

	private:

		QMutex mMutex;

		ViAudioFormat mFormat;
		ViStreamOutput mOutput;

		ViAudioObjectPointer mObject;
		qint64 mOldDuration;

};

#endif
