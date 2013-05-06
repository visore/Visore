#ifndef VIINTERPOLATIONTARGETPROVIDER_H
#define VIINTERPOLATIONTARGETPROVIDER_H

#include <vitargetprovider.h>
#include <viinterpolator.h>

class ViInterpolationTargetProvider : public ViTargetProvider
{

	public:

		ViInterpolationTargetProvider();
		ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other);
		virtual ~ViInterpolationTargetProvider();

		void setInterpolator(ViInterpolator *interpolator); // Takes ownership

		virtual qreal calculate();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViInterpolationTargetProvider* clone();

	private:

		ViInterpolator *mInterpolator;

};

#endif
