#ifndef VIPROCESSORLIST_H
#define VIPROCESSORLIST_H

#include <QList>
#include <QThreadPool>
#include "viprocessor.h"
#include "viaudioconnection.h"
#include "viaudio.h"

class ViProcessorList : public QObject
{
	Q_OBJECT

	public:

		enum Type
		{
			InputObservers,
			OutputObservers,
			DualObservers,
			InputManipulators,
			All = InputObservers | OutputObservers | DualObservers | InputManipulators
		};

		ViProcessorList();
		~ViProcessorList();

		bool add(ViAudio::Mode mode, ViProcessor *processor);
		bool remove(ViProcessor *processor);

		void clear();
		QList<ViProcessor*> processors(int type = ViProcessorList::All);

		void observeInput(const ViSampleChunk *data);
		void manipulateInput(ViSampleChunk *data);
		void observeOutput(const ViSampleChunk *data);
		void observeDual(const ViSampleChunk *data, const ViSampleChunk *data2);

	private:

		QThreadPool mThreadPool;
		QList<ViObserver*> mInputObservers;
		QList<ViModifier*> mInputModifiers;
		QList<ViObserver*> mOutputObservers;
		QList<ViDualObserver*> mDualObservers;

};

#endif
