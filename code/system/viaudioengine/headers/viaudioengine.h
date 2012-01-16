#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "vistreaminput.h"
#include "vifileinput.h"
#include "vistreamoutput.h"
#include "vispectrumanalyser.h"
#include <QFile>
#include <QBuffer>

class ViAudioEngine : public QObject
{
    Q_OBJECT

	public:
		enum ViAudioType
		{
			None = 0,
			File = 1,
			Stream = 2
		};

	signals:
		/**
		* Format of audio data has changed
		*/
		void formatChanged(const QAudioFormat &format);

		/**
		* Length of buffer has changed.
		* \param duration Duration in microseconds
		*/
		void bufferLengthChanged(qint64 duration);

		/**
		* Amount of data in buffer has changed.
		* \param Length of data in bytes
		*/
		void dataLengthChanged(qint64 duration);

		/**
		* Position of the audio input device has changed.
		* \param position Position in bytes
		*/
		void recordPositionChanged(qint64 position);

		/**
		* Position of the audio output device has changed.
		* \param position Position in bytes
		*/
		void playPositionChanged(qint64 position);

	private slots:
		void receiveNotification();

	public:
		ViAudioEngine(QObject *parent = 0);
		~ViAudioEngine();

	private:
		void initializeInput(QString filePath, QAudioFormat format);
		void initializeOutput(QAudioDeviceInfo deviceInfo, QAudioFormat format);
		void calculateSpectrum(qint64 position);
		void setPlayPosition(qint64 position, bool forceEmit = false);
		void setFormat(QAudioFormat format);

	private:
		ViAudioInput *mAudioInput;
		ViAudioOutput *mAudioOutput;
		qint64 mPlayPosition;
		int mSpectrumBufferLength;
		qint64 mDataLength;
		ViAudioEngine::ViAudioType mInputAudioType;
		ViAudioEngine::ViAudioType mOutputAudioType;
};

#endif
