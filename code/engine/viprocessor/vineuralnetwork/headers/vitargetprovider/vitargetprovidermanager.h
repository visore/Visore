#ifndef VITARGETPROVIDERMANAGER_H
#define VITARGETPROVIDERMANAGER_H

#include <QList>
#include <vitargetprovider.h>
#include <visingleton.h>
#include <vilibrarymanager.h>
#include <vielement.h>

class ViTargetProviderManager : public ViSingleton<ViTargetProviderManager>
{

	friend class ViSingleton<ViTargetProviderManager>;

	public:

		~ViTargetProviderManager();

		static ViTargetProvider* create(QString name);
		static ViTargetProvider* create(ViElement element);
		static ViTargetProvider* createDefault();

		static QString libraryPath();

	protected:

		ViTargetProviderManager();
		void addProvider(ViTargetProvider* providerr);

		static ViTargetProvider* createProvider(QString name);

	private:

		ViLibraryManager mManager;
		QList<ViTargetProvider*> mProviders;

};

#endif
