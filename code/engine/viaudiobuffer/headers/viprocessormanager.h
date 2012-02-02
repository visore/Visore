#ifndef VIPROCESSORMANAGER_H
#define VIPROCESSORMANAGER_H

#include <QSharedPointer>

class ViProcessorManager
{
	public:
		static int nextId();
		
	protected:
		ViProcessorManager();
		static ViProcessorManager* instance();
		int id();
		void incrementId();

	private:
		int mCurrentId;
		static QSharedPointer<ViProcessorManager> mInstance;
};

#endif
