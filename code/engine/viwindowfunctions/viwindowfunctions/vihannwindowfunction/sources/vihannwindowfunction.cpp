#include "vihannwindowfunction.h"

ViHannWindowFunction::ViHannWindowFunction()
	: ViWindowFunction()
{
}

ViWindowFunction* ViHannWindowFunction::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViHannWindowFunction>(new ViHannWindowFunction()).dynamicCast<ViWindowFunction>();
	}
	return mInstance.data();
}

qreal ViHannWindowFunction::calculate(qreal currentSample, qint32 totalSamples)
{
	return 0.5 * (1 - qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

QString ViHannWindowFunction::name()
{
	return "Hann Window Function";
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWindowFunction* create()
{
   return ViHannWindowFunction::instance();
}

#ifdef __cplusplus
}
#endif
