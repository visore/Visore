#ifndef VIINTERPOLATIONTARGETPROVIDER_H
#define VIINTERPOLATIONTARGETPROVIDER_H

#include <vitargetprovider.h>

class ViInterpolationTargetProvider : public ViTargetProvider
{

	public:

		ViInterpolationTargetProvider();
		ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other);
		virtual ~ViInterpolationTargetProvider();

		virtual qreal calculate(int index = -1);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViInterpolationTargetProvider* clone();

};

#endif
