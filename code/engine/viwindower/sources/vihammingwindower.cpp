#include "vihammingwindower.h"

ViHammingWindower::ViHammingWindower()
	: ViWindower()
{
}

ViWindower* ViHammingWindower::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViHammingWindower>(new ViHammingWindower()).dynamicCast<ViWindower>();
	}
	return mInstance.data();
}

qreal ViHammingWindower::calculate(qint32 currentSample, qint32 totalSamples)
{
	return 0.54 + (0.46 * qCos((2 * M_PI * currentSample) / (totalSamples - 1)));
}

QString ViHammingWindower::name()
{
	return "Hamming";
}
