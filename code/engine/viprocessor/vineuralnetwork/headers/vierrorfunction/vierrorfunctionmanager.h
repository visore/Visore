#ifndef VIERRORFUNCTIONMANAGER_H
#define VIERRORFUNCTIONMANAGER_H

#include <vierrorfunction.h>
#include <vilibrarymanager.h>
#include <visingleton.h>

class ViErrorFunctionManager : public ViSingleton<ViErrorFunctionManager>
{

	friend class ViSingleton<ViErrorFunctionManager>;

	public:

		~ViErrorFunctionManager();

		static ViErrorFunction* create(QString name);
		static ViErrorFunction* create(ViElement element);
		static ViErrorFunction* createDefault();

		static QString libraryPath();

	protected:

		ViErrorFunctionManager();
		void addFunction(ViErrorFunction *function);

		static ViErrorFunction* createFunction(QString name);

	private:

		ViLibraryManager mManager;
		QList<ViErrorFunction*> mFunctions;

};

#endif
