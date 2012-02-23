#ifndef VIENCODERMANAGER_H
#define VIENCODERMANAGER_H

#include "viaudioencoder.h"
#include "visingletonmanager.h"
#include <QSharedPointer>

class ViEncoderManager
{
	public:
		static QList<ViAudioEncoder*> all();
		static QList<ViAudioEncoder*> selected(ViAudioFormat *format);
		static QList<ViAudioEncoder*> selected(QList<QString> names);
		static ViAudioEncoder* selected(QString name);
		
	protected:
		ViEncoderManager();
		static ViEncoderManager* instance();

	protected:
		static QSharedPointer<ViEncoderManager> mInstance;
		ViSingletonManager<ViAudioEncoder> mManager;
};

#endif
