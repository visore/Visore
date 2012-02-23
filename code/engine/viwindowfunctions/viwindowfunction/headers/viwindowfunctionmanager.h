#ifndef VIWINDOWFUNCTIONMANAGER_H
#define VIWINDOWFUNCTIONMANAGER_H

#include "viwindowfunction.h"
#include "visingletonmanager.h"
#include <QSharedPointer>

class ViWindowFunctionManager
{
	public:
		static QList<ViWindowFunction*> all();
		static QList<ViWindowFunction*> selected(QList<QString> names);
		static ViWindowFunction* selected(QString name);
		
	protected:
		ViWindowFunctionManager();
		static ViWindowFunctionManager* instance();

	protected:
		static QSharedPointer<ViWindowFunctionManager> mInstance;
		ViSingletonManager<ViWindowFunction> mManager;
};

#endif
