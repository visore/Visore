#ifndef VILIBRARYMANAGER_H
#define VILIBRARYMANAGER_H

#include <QLibrary>
#include <QList>
#include <QStringList>

class ViLibraryManager
{

	public:

		ViLibraryManager(QString directory = "");
		~ViLibraryManager();

		int load(QString directory); //Returns the number of libraries loaded
		void unload();

		bool contains(QLibrary *library);
		bool contains(QString libraryPath);

		QList<QFunctionPointer> resolve(QString functionName);

		static QStringList find(QString directory);

	private:

		QList<QLibrary*> mLibraries;

};

#endif
