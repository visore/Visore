#ifndef VINOTIFIER_H
#define VINOTIFIER_H

#include <QObject>
#include <vilogger.h>

class ViNotifier : public QObject
{

	Q_OBJECT

	signals:

		void started();
		void finished();
		void progressed(qreal progress);
		void statused(QString status);

	public:

		~ViNotifier();

	protected:

		void setProgress(qreal progress);
		void setStatus(QString status);

};

#endif
