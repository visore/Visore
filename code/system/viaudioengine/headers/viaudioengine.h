#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudiobuffer.h"
#include "viaudioinput.h"
#include "viaudiooutput.h"
#include "vispectrumanalyser.h"
#include <QList>

#include "wavfile.h"

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

class ViAudioEngine : public QObject
{
    Q_OBJECT

	public:
		enum ViAudioType
		{
			None = 0,
			File = 1,
			Stream = 2,
			FileAndStream = 3
		};

	private slots:
		void changeReceived(int startIndex, int size);

	public:
		ViAudioEngine();
		~ViAudioEngine();
		//void setInputDevice

	private:
		/*void initializeInputStream(QAudioDeviceInfo deviceInfo, QAudioFormat format);
		void initializeInputFile(QString filePath);
		void initializeOutputStream();
		void initializeOutputFile();*/

	private:
		ViAudioBuffer *mBuffer;
		ViAudioInput *mAudioInput;
		QList<ViAudioOutput*> mAudioOutputs;
		ViAudioEngine::ViAudioType mInputAudioType;
		ViAudioEngine::ViAudioType mOutputAudioType;
};

#endif
