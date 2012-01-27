#ifndef VIENCODERMANAGER_H
#define VIENCODERMANAGER_H

#include "viaudioencoder.h"

class ViEncoderManager
{
	public:
		static QList<ViAudioEncoder*> encoder(ViAudioFormat *format);
		static ViAudioEncoder* encoder(QString name);
		~ViEncoderManager(); //Must be public for QSharedPointer
		
	protected:
		ViEncoderManager();
		static ViEncoderManager* instance();

	protected:
		QList<ViAudioEncoder*> mEncoders;
		QList<ViLibrary<ViAudioEncoder>*> mLibraries;
		static QSharedPointer<ViEncoderManager> mInstance;
};

#endif
