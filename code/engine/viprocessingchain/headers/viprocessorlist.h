#ifndef VIPROCESSORLIST_H
#define VIPROCESSORLIST_H

#include <QList>
#include <QThreadPool>
#include "viprocessor.h"
#include "viaudioconnection.h"

class ViProcessorList : public QObject
{
	Q_OBJECT

	private slots:

		void updateActiveProcessors();

	public:

		ViProcessorList();
		~ViProcessorList();
		bool add(ViAudioConnection::Direction direction, ViProcessor *processor);
		void removeAll();
		QList<ViProcessor*> processors();

		void observeInput(const ViSampleChunk *data);
		void manipulateInput(ViSampleChunk *data);
		void observeOutput(const ViSampleChunk *data);

	private:

		QThreadPool mThreadPool;

		QList<ViObserver*> mInputObservers;
		QList<ViModifier*> mInputModifiers;
		QList<ViObserver*> mOutputObservers;

		QList<ViObserver*> mActiveInputObservers;
		QList<ViModifier*> mActiveInputModifiers;
		QList<ViObserver*> mActiveOutputObservers;

};

#endif
