#include "vinowindowfunction.h"

ViNoWindowFunction::ViNoWindowFunction()
	: ViWindowFunction()
{
}

ViWindowFunction* ViNoWindowFunction::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViNoWindowFunction>(new ViNoWindowFunction()).dynamicCast<ViWindowFunction>();
	}
	return mInstance.data();
}

qreal ViNoWindowFunction::calculate(qreal currentSample, qint32 totalSamples)
{
	return 1.0;
}

QString ViNoWindowFunction::name()
{
	return "No Window Function";
}

#ifdef __cplusplus
extern "C"
{
#endif

ViWindowFunction* create()
{
   return ViNoWindowFunction::instance();
}

#ifdef __cplusplus
}
#endif
