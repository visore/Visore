#ifndef VIUNDERRUNHANDLER_H
#define VIUNDERRUNHANDLER_H

#include "vihandler.h"
#include <QTimer>
#include <QMutex>

class ViUnderrunHandler : public ViHandler
{

    Q_OBJECT

	signals:

		void started();
		void progressed(short progress);
		void finished();

	private slots:

		void changeStreamOutput(ViStreamOutput *output);
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
		ViAudioFormat mFormat;
		ViStreamOutput *mOutput;
		int mSecondsPassed;
		int mSecondsNeeded;
		bool mBufferingStarted;

};

#endif
