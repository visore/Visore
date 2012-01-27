#ifndef VIFORMATMANAGER_H
#define VIFORMATMANAGER_H

#include "viaudioformat.h"
#include "viessentials.h"

class ViFormatManager
{
	public:
		static QList<ViAudioFormat*> formats(QList<QString> names); 
		static ViAudioFormat* format(QString name);
		~ViFormatManager(); //Must be public for QSharedPointer
		
	protected:
		ViFormatManager();
		static ViFormatManager* instance();

	protected:
		QList<ViAudioFormat*> mFormats;
		QList<ViLibrary<ViAudioFormat>*> mLibraries;
		static QSharedPointer<ViFormatManager> mInstance;
};

#endif
