#ifndef VIPROCESSORLIST_H
#define VIPROCESSORLIST_H

#include <QList>
#include <QThreadPool>
#include "viprocessor.h"
#include "viaudioconnection.h"

class ViProcessorList : public QObject
{
	Q_OBJECT

	public:

		ViProcessorList();
		~ViProcessorList();

		bool add(ViAudioConnection::Direction direction, ViProcessor *processor);
		bool remove(ViProcessor *processor);

		void clear();
		QList<ViProcessor*> all();

		void observeInput(const ViSampleChunk *data);
		void manipulateInput(ViSampleChunk *data);
		void observeOutput(const ViSampleChunk *data);

	private:

		QThreadPool mThreadPool;
		QList<ViObserver*> mInputObservers;
		QList<ViModifier*> mInputModifiers;
		QList<ViObserver*> mOutputObservers;

};

#endif
