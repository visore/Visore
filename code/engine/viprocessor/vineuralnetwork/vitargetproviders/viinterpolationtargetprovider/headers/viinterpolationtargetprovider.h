#ifndef VIINTERPOLATIONTARGETPROVIDER_H
#define VIINTERPOLATIONTARGETPROVIDER_H

#include <vitargetprovider.h>
#include <vichunk.h>

class ViInterpolationTargetProvider : public ViTargetProvider
{

	public:

		ViInterpolationTargetProvider();
		ViInterpolationTargetProvider(const ViInterpolationTargetProvider &other);
		virtual ~ViInterpolationTargetProvider();

		virtual void setData(ViSampleChunk chunk);

		virtual qreal calculate();

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViInterpolationTargetProvider* clone();

	private:

		ViSampleChunk mChunk;

};

#endif
