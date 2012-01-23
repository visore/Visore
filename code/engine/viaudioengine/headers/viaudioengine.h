#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viobject.h"
#include "viaudioconnection.h"
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
			Stream = 2,
			FileAndStream = 3
		};

	public slots:
		void startInput();
		void pauseInput();
		void stopInput();
		void startOutput();
		void pauseOutput();
		void stopOutput();

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
		ViAudioConnection *mAudioConnection;
		ViLibrary<ViAudioConnection> *mAudioConnectionLoader;
		ViAudioBuffer *mBuffer;
		ViAudioInput *mAudioInput;
		QList<ViAudioOutput*> mAudioOutputs;
		ViAudioEngine::ViAudioType mInputAudioType;
		ViAudioEngine::ViAudioType mOutputAudioType;
};

#endif
