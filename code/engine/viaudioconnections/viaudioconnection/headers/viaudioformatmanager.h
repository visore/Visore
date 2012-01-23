#ifndef VIAUDIOFORMATMANAGER_H
#define VIAUDIOFORMATMANAGER_H

#include "viaudioformat.h"
#include "viessentials.h"

class ViAudioFormatManager
{
	public:
		static ViAudioFormatManager* initialize();
		static void close();
		ViAudioFormat* format(QString name);
		QList<ViAudioFormat*>* formats(QList<QString> names);

	protected:
		ViAudioFormatManager();
		~ViAudioFormatManager();

	private:
		static ViAudioFormatManager *mInstance;
		QList<ViLibrary<ViAudioFormat>*> mLibraries;
		QList<ViAudioFormat*> mFormats;
};

#endif
