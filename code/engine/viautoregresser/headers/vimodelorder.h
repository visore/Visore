#ifndef VIMODELORDER_H
#define VIMODELORDER_H

#include <vilibrarymanager.h>

class ViModelOrder : public ViLibrary
{

	public:

		ViModelOrder();
		ViModelOrder(const ViModelOrder &other);
		virtual ~ViModelOrder();

		virtual ViModelOrder* clone() = 0;

		virtual void clear();

		void setParameterCount(const int &parameters);
		void setSampleCount(const int &count);

		virtual int order() const = 0;

	protected:

		int mParameterCount;
		int mSampleCount;

};

class ViModelOrderManager : public ViLibraryManager<ViModelOrderManager, ViModelOrder>
{

	public:

		QString libraryPath();
		QString defaultLibrary();

};

#endif
