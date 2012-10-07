#ifndef VIUNDERRUNHANDLER_H
#define VIUNDERRUNHANDLER_H

#include "vihandler.h"
#include <QTimer>
#include <QMutex>

class ViUnderrunHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void changeOutput(ViStreamOutput *output);
		void handleUnderrun();
		void calculate(qreal processingRate);
		void update();

	public:

		ViUnderrunHandler(ViProcessingChain *chain);

	private:

		int secondsNeeded(qreal processingRate);

	private:

		QTimer mTimer;
		QMutex mMutex;
		int mSecondsPassed;
		int mSecondsNeeded;
		bool mBufferingStarted;

};

#endif
