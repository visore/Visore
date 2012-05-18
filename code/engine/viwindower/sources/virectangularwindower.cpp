#include "virectangularwindower.h"

ViRectangularWindower::ViRectangularWindower()
	: ViWindower()
{
}

ViWindower* ViRectangularWindower::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViRectangularWindower>(new ViRectangularWindower()).dynamicCast<ViWindower>();
	}
	return mInstance.data();
}

qreal ViRectangularWindower::calculate(qint32 currentSample, qint32 totalSamples)
{
	return 1.0;
}

QString ViRectangularWindower::name()
{
	return "Rectangular";
}
