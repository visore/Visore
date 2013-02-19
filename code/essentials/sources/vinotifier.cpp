#include <vinotifier.h>

ViNotifier::~ViNotifier()
{
}

void ViNotifier::setProgress(qreal progress)
{
	emit progressed(progress);
}

void ViNotifier::setStatus(QString status)
{
	emit statused(status);
}
