#ifndef VIQTFILEOUTPUT_H
#define VIQTFILEOUTPUT_H

#include "vifileoutput.h"
#include "vicoder.h"
#include <QFile>

class ViQtFileOutput : public ViFileOutput
{
	Q_OBJECT

	public:

		ViQtFileOutput();
		~ViQtFileOutput();
		void initialize();
		void free();

		void start();
		void stop();
		void pause();

	private:
		ViCoder mCoder;
};

#endif
