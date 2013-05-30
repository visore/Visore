#ifndef VILIBRARYMANAGER_H
#define VILIBRARYMANAGER_H

#include <QLibrary>
#include <QDir>
#include <QList>
#include <QStringList>
#include <vilibrary.h>
#include <vipointer.h>
#include <visingleton.h>
#include <vilogger.h>
#include <vielement.h>

#define VI_DEFAULT_LIBRARY_FUNCTION "create"

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
class ViLibraryManager : public ViSingleton<MANAGER_TYPE>
{

	friend class ViSingleton<MANAGER_TYPE>;
	typedef ViPointer<MANAGER_TYPE> ViLibraryManagerPointer;

	public:

		~ViLibraryManager();

		static LIBRARY_TYPE* create(QString name, QString functionName = VI_DEFAULT_LIBRARY_FUNCTION);
		static LIBRARY_TYPE* create(ViElement element, QString functionName = VI_DEFAULT_LIBRARY_FUNCTION);
		static LIBRARY_TYPE* createDefault(QString functionName = VI_DEFAULT_LIBRARY_FUNCTION);

		static QList<LIBRARY_TYPE*> libraries(QString functionName = VI_DEFAULT_LIBRARY_FUNCTION);
		static QStringList names(QString functionName = VI_DEFAULT_LIBRARY_FUNCTION);

		virtual QString libraryPath() = 0;
		virtual QString defaultLibrary() = 0;

	protected:

		ViLibraryManager();

		LIBRARY_TYPE* createLibrary(QString name);
		
		int load(QString directory, QString functionName); //Returns the number of libraries loaded
		void unload();

		bool contains(QLibrary *library);
		bool contains(QString libraryPath);

		QStringList find(QString directory);
		QString trailer();

	private:

		bool mWasLoaded;
		QList<QLibrary*> mLibraryFiles;
		QList<ViLibrary*> mLibraries;

};

#include <vilibrarymanager.cpp>

#endif
