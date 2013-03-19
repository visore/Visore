#ifndef VIWEIGHTINITIALIZERMANAGER_H
#define VIWEIGHTINITIALIZERMANAGER_H

#include <QList>
#include <viweightinitializer.h>
#include <visingleton.h>
#include <vilibrarymanager.h>
#include <vielement.h>

class ViWeightInitializerManager : public ViSingleton<ViWeightInitializerManager>
{

	friend class ViSingleton<ViWeightInitializerManager>;

	public:

		~ViWeightInitializerManager();

		static ViWeightInitializer* create(QString name);
		static ViWeightInitializer* create(ViElement element);
		static ViWeightInitializer* createDefault();

		static QString libraryPath();

	protected:

		ViWeightInitializerManager();
		void addInitializer(ViWeightInitializer* initializer);

		static ViWeightInitializer* createInitializer(QString name);

	private:

		ViLibraryManager mManager;
		QList<ViWeightInitializer*> mInitializers;

};

#endif
