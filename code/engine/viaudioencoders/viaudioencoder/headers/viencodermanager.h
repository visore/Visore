#ifndef VIENCODERMANAGER_H
#define VIENCODERMANAGER_H

#include "viaudioencoder.h"

class ViEncoderManager
{
	public:
		static QList<ViAudioEncoder*> findEncoder(ViAudioFormat *format);
		static ViAudioEncoder* findEncoder(QString name);
		
	protected:
		ViEncoderManager();
		static ViEncoderManager* instance();

	protected:
		QList<ViAudioEncoder*> mEncoders;
		static QSharedPointer<ViEncoderManager> mInstance;
};

#endif
