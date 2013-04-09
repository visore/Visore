#ifndef VITRAINERMANAGER_H
#define VITRAINERMANAGER_H

#include <QList>
#include <vitrainer.h>
#include <visingleton.h>
#include <vilibrarymanager.h>

class ViTrainerManager : public ViSingleton<ViTrainerManager>
{

	friend class ViSingleton<ViTrainerManager>;

	public:

		~ViTrainerManager();

		static ViTrainer* create(QString name);
		static ViTrainer* createDefault();

		static QString libraryPath();

	protected:

		ViTrainerManager();
		void addTrainer(ViTrainer* trainer);

		static ViTrainer* createTrainer(QString name);

	private:

		ViLibraryManager mManager;
		QList<ViTrainer*> mTrainers;

};

#endif
