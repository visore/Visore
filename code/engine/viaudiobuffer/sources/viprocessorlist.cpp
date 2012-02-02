#include "viprocessorlist.h"

void ViProcessorList::append(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution)
{
	mExecutions.append(execution);
	mProcessors.append(processor);
}

bool ViProcessorList::processors(int id, QList<ViProcessor*> *list)
{
	if(id < size())
	{
		list->append(mProcessors[id]);
		++id;
		if(mExecutions[id-1] == ViProcessorList::Parallel)
		{
			while(id < size() && mExecutions[id] == ViProcessorList::Parallel)
			{
				list->append(mProcessors[id]);
				++id;
			}
			if(id < size() && mExecutions[id] == ViProcessorList::Sequential)
			{
				list->append(mProcessors[id]);
				++id;
			}
		}
	}
	if(id == size() && mExecutions[id-1] == ViProcessorList::Parallel)
	{
		return true;
	}
	return false;
}


void ViProcessorList::clear()
{
	mProcessors.clear();
	mExecutions.clear();
}

int ViProcessorList::size()
{
	return mExecutions.size();
}
