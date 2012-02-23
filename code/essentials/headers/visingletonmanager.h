#ifndef VISINGLETONMANAGER_H
#define VISINGLETONMANAGER_H

#include "vilibrary.h"
#include "vilibrarydetector.h"
#include "visingleton.h"

template <class T = ViSingleton>
class ViSingletonManager
{
	public:
		QList<T*> all();
		QList<T*> selected(QList<QString> names); 
		T* selected(QString name);
		void searchPath(QString path);
		QList<T*>* singletons();
		~ViSingletonManager();

	protected:
		QList<T*> mSingletons;
		QList<ViLibrary<T>*> mLibraries;
};

//Template decleration and implementation should actually be in the same file
#include "../sources/visingletonmanager.cpp"

#endif
