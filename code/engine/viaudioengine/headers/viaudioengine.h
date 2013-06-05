#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"
#include "vimanager.h"

#include "viaudioobjectchain.h"

#include <viaudiorecorder.h>

class ViAudioEngine : public QObject, public ViSingleton<ViAudioEngine>
{

    Q_OBJECT

	friend class ViSingleton<ViAudioEngine>;

	public slots:

		//Input
		void changeInput(ViAudio::Input input);

		//Playback
		void startPlayback();
		void stopPlayback();
		void pausePlayback();
		void setPosition(int seconds);

		//Recording
		void startRecording();
		void stopRecording();
		void pauseRecording();

		//File I/O
		void openFile(QString filePath);
		void saveFile(QString filePath);

		//Spectrum
		void calculateSpectrum(qint32 size, QString windowFunction);

		//Correlation
		void calculateCorrelation(ViAudioObjectPointer object);

		//Correction
		void correct(ViAudioObjectQueue objects, ViModifyProcessor *corrector);

		//Project
		void recordProject(ViProject *project, ViAudioObject::Type type, ViAudioFormat format, int sides, bool detectInfo);

		//Volume
		void setVolume(int volume);
		void mute(bool value = true);
		void unmute();

		//Analyse
		void generateWaveForm(ViAudioObjectPointer object, ViAudioObject::Type type = ViAudioObject::All);

		//Align
		void align(ViProject &project);
		void align(ViAudioObjectPointer object);

	private slots:

		void disconnectObject();

	signals:

		//General
		void progressed(qreal progress);
		void progressFinished();
		void statusChanged(QString status);

		//Chain
		void chainChanged();
		void buffering(short progress);

		//Playback
		void positionChanged(ViAudioPosition position);
		void lengthChanged(ViAudioPosition length);

		//Input
		void inputChanged(ViAudio::Input input);		

		//Song detector
		void songDetected(ViSongInfo info);

		//Spectrum
		void spectrumChanged(ViRealSpectrum spectrum, qint64 milliseconds);
		void spectrumProgressed(short percentage);
		void spectrumFinished();

		//Correlation
		void correlationProgressed(short percentage);
		void correlationFinished();

		//Project
		void loadProjectStarted();
		void saveProjectStarted();
		void projectFinished();

		//Volume
		void volumeChanged(int volume);
		void muted();
		void unmuted();

	public:

		~ViAudioEngine();

		//Volume
		int volume();

	protected:

		ViAudioEngine();

	private:

		ViAudioConnection mConnection;
		ViFileInput *mFileInput;
		ViFileOutput *mFileOutput;
		ViStreamInput *mStreamInput;
		ViStreamOutput *mStreamOutput;

		ViAudioObjectChain mObjectChain;

		ViAudioRecorder mRecorder;

};

typedef ViPointer<ViAudioEngine> ViAudioEnginePointer;

#endif
