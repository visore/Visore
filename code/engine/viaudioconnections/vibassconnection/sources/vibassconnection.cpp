#include "vibassconnection.h"

ViBassConnection::ViBassConnection()
	: ViAudioConnection()
{
	initialize();
}

ViBassConnection::~ViBassConnection()
{
	close();
}

ViFileInput* ViBassConnection::fileInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QString filePath)
{
	if(mFileInput == NULL)
	{
		mFileInput = new ViBassFileInput(buffer, metaData, filePath);
	}
	return mFileInput;
}

ViStreamInput* ViBassConnection::streamInput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{
	if(mStreamInput == NULL)
	{
		mStreamInput = new ViBassStreamInput(buffer, metaData, device);
	}
	return mStreamInput;
}

//ViFileOutput* ViBassConnection::fileOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, QString filePath);

ViStreamOutput* ViBassConnection::streamOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
{
	if(mStreamOutput == NULL)
	{
		mStreamOutput = new ViBassStreamOutput(buffer, metaData, device);
	}
	return mStreamOutput;
}

void ViBassConnection::initialize()
{
	mFormatManager = ViAudioFormatManager::initialize();
	//populateInputFormats();
	//populateOutputFormats();
	/*QList<QString> pluginPaths = ViLibraryDetector::detectLibraries(BASSPLUGINSLOCATION, BASSLIBRARYLOCATION);
	for(int i = 0; i < pluginPaths.length(); ++i)
	{
		HPLUGIN plugin = BASS_PluginLoad(pluginPaths[i].toUtf8().data(), 0);
		if(!plugin)
		{
			setErrorParameters("ViBassConnection - Plugin Error", "Could not load the plugin (" + pluginPaths[i].toUtf8().data() + ")", ViErrorInfo::Fatal);
		}
		else
		{
			mPlugins.append(plugin);
		}
	}*/
	if(!BASS_Init(-1, 44100, 0, 0, 0))
	{
		setErrorParameters("ViBassConnection - Initializing Error", "Could not initialize Bass resources", ViErrorInfo::Fatal);
	}
}

void ViBassConnection::close()
{
	ViAudioFormatManager::close();
	if(!BASS_Free())
	{
		setErrorParameters("ViBassConnection - Closing Error", "Could not close Bass resources", ViErrorInfo::Fatal);
	}
	/*for(int i = 0; i < plugins.length(); ++i)
	{
		if(!BASS_PluginFree(mPlugins[i]))
		{
			setErrorParameters("ViBassConnection - Plugin Error", "Could not unload the specified plugin", ViErrorInfo::Fatal);
		}
	}*/
}

void ViBassConnection::populateInputFormats()
{
	QList<QString> formats;
	formats.append("MP3");
	formats.append("WAVE");
	mInputFormats = mFormatManager->formats(formats);
}

void ViBassConnection::populateOutputFormats()
{
	QList<QString> formats;
	formats.append("MP3");
	formats.append("WAVE");
	mOutputFormats = mFormatManager->formats(formats);
}

QList<ViAudioDevice> ViBassConnection::devices(ViAudioDevice::ViAudioDeviceType type)
{
	return ViBassDevice::devices(type);
}

ViAudioDevice ViBassConnection::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{
	return ViBassDevice::defaultDevice(type);
}

/*******************************************
ViBassConnection
********************************************/

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioConnection* createConnection()
{
   return new ViBassConnection();
}

void deleteConnection(ViBassConnection *connection)
{
   delete connection;
}

#ifdef __cplusplus
}
#endif
