#include "vihannwindower.h"

ViHannWindower::ViHannWindower()
	: ViWindower()
{
}

ViWindower* ViHannWindower::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViHannWindower>(new ViHannWindower()).dynamicCast<ViWindower>();
	}
	return mInstance.data();
}

qreal ViHannWindower::calculate(qint32 currentSample, qint32 totalSamples)
{
	return 0.5 * (1 - qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

QString ViHannWindower::name()
{
	return "Hann";
}
