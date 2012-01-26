#include "viencodermanager.h"

ViEncoderManager::ViEncoderManager()
{

}

ViEncoderManager* ViEncoderManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViEncoderManager>(new ViEncoderManager);
	}
	return mInstance.data();
}

QList<ViAudioEncoder*> ViEncoderManager::findEncoder(ViAudioFormat *format)
{
	/*ViEncoderManager *manager = ViEncoderManager::instance();
	QList<ViAudioEncoder*> result;
	for(int i = 0; i < manager->encoders().size(); ++i)
	{
		if(format == manager->encoders()[i]->format())
		{
			result.append(manager->encoders()[i]);
		}
	}
	return result;*/
}

ViAudioEncoder* ViEncoderManager::findEncoder(QString name)
{
	/*ViEncoderManager *manager = ViEncoderManager::instance();
	for(int i = 0; i < manager->encoders().size(); ++i)
	{
		if(name == manager->encoders()[i]->name())
		{
			return manager->encoders()[i];
		}
	}
	return NULL;*/
}
