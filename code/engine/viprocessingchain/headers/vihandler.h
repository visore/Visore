#ifndef VIHANDLER_H
#define VIHANDLER_H

#include "viprocessingchain.h"

class ViHandler : public QObject
{

    Q_OBJECT

	signals:

		void started();
		void progressed(short progress);
		void finished();

	public:

		ViHandler(ViProcessingChain *chain);
		ViProcessingChain* chain();

		virtual void enable(bool enable = true);
		virtual void disable();
		bool isEnabled();
		
		void enableAll(bool enable = true, bool exceptMe = true);
		void disableAll(bool exceptMe = true);

	protected:

		ViProcessingChain *mChain;
		bool mIsEnabled;

};

#endif
