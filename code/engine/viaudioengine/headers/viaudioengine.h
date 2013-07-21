#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"
#include "vimanager.h"

#include "viaudioobjectchain.h"

#include <viaudiorecorder.h>
#include <viaudioplayer.h>

class ViAudioEngine : public QObject, public ViSingleton<ViAudioEngine>
{

    Q_OBJECT

	friend class ViSingleton<ViAudioEngine>;

	public slots:

		//Playback
		void playback(ViAudioObjectPointer object, ViAudio::Type type);
		void clearPlayback();
		void startPlayback();
		void stopPlayback();
		void pausePlayback();
		void setPlaybackPosition(ViAudioPosition position);
		void setPlaybackVolume(int volume);

		//Correlation
		void correlate(ViAudioObjectPointer object);
		void correlate(ViAudioObjectQueue objects);

		//Correction
		void correct(ViAudioObjectQueue objects, ViModifyProcessor *corrector);

		//Project
		void recordProject(ViProject *project, ViAudio::Type type, ViAudioFormat format, int sides, bool detectInfo);

		//Metadata
		void updateMetadata(ViProject *project);

		//Analyse
		void generateWave(ViAudioObjectPointer object, ViAudio::Type type = ViAudio::All, const bool &align = false);
		void generateSpectrum(ViAudioObjectPointer object, ViAudio::Type type, qint32 windowSizeSamples, QString windowFunction);

		//Align
		void align(ViProject &project);
		void align(ViAudioObjectPointer object);

	signals:

		//General
		void progressed(qreal progress);
		void progressFinished();
		void statusChanged(QString status);

		//Playback
		void positionChanged(ViAudioPosition position);
		void durationChanged(ViAudioPosition duration);

		//Spectrum
		void spectrumChanged(ViRealSpectrum spectrum, qint64 milliseconds);
		void spectrumProgressed(short percentage);
		void spectrumFinished();

		//Correlation
		void correlationProgressed(short percentage);
		void correlationFinished();

	public:

		~ViAudioEngine();

	protected:

		ViAudioEngine();

	private:

		ViAudioObjectChain mObjectChain;
		ViAudioRecorder mRecorder;
		ViAudioPlayer mPlayer;

};

typedef ViPointer<ViAudioEngine> ViAudioEnginePointer;

#endif
