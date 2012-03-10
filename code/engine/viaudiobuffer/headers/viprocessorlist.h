#ifndef VIPROCESSORLIST_H
#define VIPROCESSORLIST_H

#include "viprocessor.h"
#include <QList>

class ViProcessorList
{
	public:
		enum ViProcessorExecution
		{
			Sequential = 0,
			Parallel = 1
		};
		void append(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution);
		bool processors(int id, QList<ViProcessor*> *list); //return true if the last element in list if Parallel and also the last in mProcessors
		void reset();
		void clear();
		int size();

	protected:
		QList<ViProcessorList::ViProcessorExecution> mExecutions;
		QList<ViProcessor*> mProcessors;
};

#endif
