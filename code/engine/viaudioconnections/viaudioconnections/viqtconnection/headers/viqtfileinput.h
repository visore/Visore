#ifndef VIQTFILEINPUT_H
#define VIQTFILEINPUT_H

#include "vifileinput.h"
#include "vicoder.h"

class ViQtFileInput : public ViFileInput
{
	Q_OBJECT

	public:

		ViQtFileInput();

		void start();
		void stop();
		void pause();

	private:

		ViCoder mCoder;

};

#endif

