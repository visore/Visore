#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viobject.h"
#include "viaudioconnection.h"
#include "viaudioprocessingchain.h"
#include "viwaveformer.h"
#include "vispectrumanalyzer.h"
//#include "visongdetector.h"
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
		void originalWaveChanged(QSharedPointer<ViWaveFormChunk> chunk);
		void correctedWaveChanged(QSharedPointer<ViWaveFormChunk> chunk);
		void originalBufferChanged(int size);
		void correctedBufferChanged(int size);
		void positionChanged(ViAudioPosition position);
		void inputChanged(ViAudioEngine::ViAudioType type);

	public:
		ViAudioEngine();
		~ViAudioEngine();
		ViAudioProcessingChain* processingChain();
		void setInput(ViAudioEngine::ViAudioType type);
		void setInputFilePath(QString filePath);
		void setOutputFilePath(QString filePath);
		void reset();
		int volume();
		void calculateWaveForm(ViAudioBuffer::ViAudioBufferType type, qint64 start, qint64 length);

	private:
		/*void initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format);
		void initializeInputFile(QString filePath);
		void initializeOutputStream();
		void initializeOutputFile();*/
		void resetMetaData();

	private:
		ViAudioConnection *mAudioConnection;
		ViLibrary<ViAudioConnection> *mAudioConnectionLoader;
		ViStreamInput *mStreamInput;
		ViFileInput *mFileInput;
		ViStreamOutput *mStreamOutput;
		ViFileOutput *mFileOutput;
		ViAudioProcessingChain *mProcessingChain;
		ViAudioMetaData *mMetaData;

		ViWaveFormer *mOriginalWaveFormer;
		ViWaveFormer *mCorrectedWaveFormer;
		ViSpectrumAnalyzer *mOriginalSpectrumAnalyzer;

		ViAudioEngine::ViAudioType mInputType;

		//ViSongDetector *mSongDetector;
};

#endif
