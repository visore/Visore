#ifndef VIINTERPOLATIONCORRECTOR_H
#define VIINTERPOLATIONCORRECTOR_H

#include <viprocessor.h>
#include <viinterpolator.h>

class ViInterpolationCorrector : public ViModifyProcessor
{

	public:

		ViInterpolationCorrector(ViInterpolator *interpolator = NULL); // Takes ownership
		virtual ~ViInterpolationCorrector();

		void clear();

		void setInterpolator(ViInterpolator *interpolator); // Takes ownership

		void initialize();
		void execute(const int &channel);
		void finalize();

		ViElement exportData();
		bool importData(ViElement element);

	private:

		ViInterpolator *mInterpolator;

};

#endif
