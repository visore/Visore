#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viobject.h"
#include "viaudioconnection.h"
#include "viprocessingchain.h"
#include "vilibrary.h"
#include "viaudioformat.h"

#include "visongdetector.h"
#include "visignalmanipulator.h"
#include "vicorrelator.h"
#include "visingleton.h"

#include "vispectrumanalyzer.h"
#include "viwaveformer.h"

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

class ViAudioEngine : public ViSingleton, public ViError
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

		void calculateSpectrum(qint32 size, QString windowFunction);
		void calculateCorrelation();

	signals:

		void inputChanged();
		void outputChanged();

		void positionChanged(ViAudioPosition position);
		void inputChanged(ViAudioEngine::ViAudioType type);

		void inputWaveChanged(ViWaveForm *waveForm);
		void outputWaveChanged(ViWaveForm *waveForm);

		void spectrumChanged(qreal percentage);
		void spectrumFinished();

		void correlationChanged(qreal percentage);
		void correlationFinished();

		void songInfoChanged(ViSongInfo info);

	public:
		~ViAudioEngine();
		static ViAudioEngine* instance();
		ViProcessingChain* processingChain();
		void setInput(ViAudioEngine::ViAudioType type);
		void setInputFilePath(QString filePath);

		void createOutputFile(QString filePath, ViAudioFormat format);

		void reset();
		int volume();

		//ViWaveForm* waveSummary(ViAudioBuffer::ViAudioBufferType type);

		ViRealSpectrum spectrum();
		ViCorrelationResult& correlation();

	private:
		/*void initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format);
		void initializeInputFile(QString filePath);
		void initializeOutputStream();
		void initializeOutputFile();*/
		void resetMetaData();

	protected:

		ViAudioEngine();

	private:

		static ViAudioEngine *mEngine;

		ViProcessingChain *mProcessingChain;

		ViSpectrumAnalyzer *mSpectrumAnalyzer;
		
		
		ViAudioFormat mFormat;

		ViAudioEngine::ViAudioType mInputType;

	//	ViWaveSummarizer mOriginalWaveSummarizer;
	//	ViWaveSummarizer mCorrectedWaveSummarizer;

		ViSongDetector *mSongDetector;

		ViCorrelator *mCorrelator;
};

#endif
