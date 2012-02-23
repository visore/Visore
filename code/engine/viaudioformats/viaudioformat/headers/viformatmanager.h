#ifndef VIFORMATMANAGER_H
#define VIFORMATMANAGER_H

#include "viaudioformat.h"
#include "visingletonmanager.h"
#include <QSharedPointer>

class ViFormatManager
{
	public:
		static QList<ViAudioFormat*> all();
		static QList<ViAudioFormat*> selected(QList<QString> names);
		static ViAudioFormat* selected(QString name);
		
	protected:
		ViFormatManager();
		static ViFormatManager* instance();

	protected:
		static QSharedPointer<ViFormatManager> mInstance;
		ViSingletonManager<ViAudioFormat> mManager;
};

#endif
