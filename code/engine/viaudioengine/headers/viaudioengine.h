#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viobject.h"
#include "viaudioconnection.h"
#include "viaudioprocessingchain.h"
#include "viwaveformer.h"
#include "vilibrary.h"
#include "viwavesummarizer.h"
#include "viaudioformat.h"
#include "vispectrumanalyzer.h"
#include "visongdetector.h"
#include "visignalmanipulator.h"
#include "vimatcher.h"
#include <QList>
#include <QCoreApplication>

struct ViAudioInputDevice
{
	public:
		ViAudioInputDevice(QString name = "", QString description = "")
		{
			mName = name;
			mDescription = description;
		}
		QString name()
		{
			return mName;
		}
		QString description()
		{
			return mDescription;
		}
		void setName(QString name)
		{
			mName = name;
		}
		void setDescription(QString description)
		{
			mDescription = description;
		}
	private:
		QString mName;
		QString mDescription;
};

class ViAudioEngine : public QObject, public ViError
{
    Q_OBJECT

	public:
		enum ViAudioType
		{
			None = 0,
			File = 1,
			Stream = 2
		};

	public slots:
		void startPlayback();
		void stopPlayback();
		void pausePlayback();
		void startRecording();
		void stopRecording();
		void setStreamPosition(ViAudioPosition position);
		void startOutputFile();
		void stopOutputFile();
		void setVolume(int volume);
		void mute(bool value = true);

	signals:
		void originalBufferChanged(int size);
		void correctedBufferChanged(int size);
		void positionChanged(ViAudioPosition position);
		void inputChanged(ViAudioEngine::ViAudioType type);

		void originalWaveChanged();
		void correctedWaveChanged();

		void songInfoChanged(ViSongInfo info);

	public:
		ViAudioEngine();
		~ViAudioEngine();
		ViAudioProcessingChain* processingChain();
		void setInput(ViAudioEngine::ViAudioType type);
		void setInputFilePath(QString filePath);

		void createOutputFile(QString filePath, ViAudioFormat format);

		void reset();
		int volume();

		ViWaveForm* waveSummary(ViAudioBuffer::ViAudioBufferType type);

	private:
		/*void initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format);
		void initializeInputFile(QString filePath);
		void initializeOutputStream();
		void initializeOutputFile();*/
		void resetMetaData();

	//private:
public:
		ViAudioConnection *mAudioConnection;
		ViLibrary<ViAudioConnection> *mAudioConnectionLoader;
		ViStreamInput *mStreamInput;
		ViFileInput *mFileInput;
		ViStreamOutput *mStreamOutput;
		ViFileOutput *mFileOutput;
		ViAudioProcessingChain *mProcessingChain;
		
		
		ViAudioFormat mFormat;

		ViAudioEngine::ViAudioType mInputType;

		ViWaveSummarizer mOriginalWaveSummarizer;
		ViWaveSummarizer mCorrectedWaveSummarizer;

		ViSongDetector *mSongDetector;
};

#endif
