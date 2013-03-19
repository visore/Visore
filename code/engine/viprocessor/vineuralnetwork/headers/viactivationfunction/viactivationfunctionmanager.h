#ifndef VIACTIVATIONFUNCTIONMANAGER_H
#define VIACTIVATIONFUNCTIONMANAGER_H

#include <QList>
#include <viactivationfunction.h>
#include <visingleton.h>
#include <vilibrarymanager.h>
#include <vielement.h>

class ViActivationFunctionManager : public ViSingleton<ViActivationFunctionManager>
{

	friend class ViSingleton<ViActivationFunctionManager>;

	public:

		~ViActivationFunctionManager();

		static ViActivationFunction* create(QString name);
		static ViActivationFunction* create(ViElement element);
		static ViActivationFunction* createDefault();

		static QString libraryPath();

	protected:

		ViActivationFunctionManager();
		void addFunction(ViActivationFunction* function);

		static ViActivationFunction* createFunction(QString name);

	private:

		ViLibraryManager mManager;
		QList<ViActivationFunction*> mFunctions;

};

#endif
