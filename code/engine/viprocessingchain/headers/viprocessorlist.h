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

		void processRawInputObservers(ViChunk<char> *data);
		void processRealInputObservers(ViChunk<double> *data);
		void processRawInputModifiers(ViChunk<char> *data);
		void processRealInputModifiers(ViChunk<double> *data);
		void processRawOutputObservers(ViChunk<char> *data);
		void processRealOutputObservers(ViChunk<double> *data);

	protected:

		bool addRaw(ViAudioConnection::Direction direction, ViRawProcessor *processor);
		bool addReal(ViAudioConnection::Direction direction, ViRealProcessor *processor);

	private:

		QThreadPool mThreadPool;

		QList<ViRawProcessor*> mRawInputObservers;
		QList<ViRealProcessor*> mRealInputObservers;
		QList<ViRawProcessor*> mRawInputModifiers;
		QList<ViRealProcessor*> mRealInputModifiers;
		QList<ViRawProcessor*> mRawOutputObservers;
		QList<ViRealProcessor*> mRealOutputObservers;

		QList<ViRawProcessor*> mActiveRawInputObservers;
		QList<ViRealProcessor*> mActiveRealInputObservers;
		QList<ViRawProcessor*> mActiveRawInputModifiers;
		QList<ViRealProcessor*> mActiveRealInputModifiers;
		QList<ViRawProcessor*> mActiveRawOutputObservers;
		QList<ViRealProcessor*> mActiveRealOutputObservers;

};

#endif
