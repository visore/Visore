#ifndef VIACOUSTIDFINGERPRINTER_H
#define VIACOUSTIDFINGERPRINTER_H

#include <vifingerprinter.h>
#include <viaudiocoder.h>

class ViAcoustidFingerprinterThread : public ViFingerprinterThread
{

	public:

		ViAcoustidFingerprinterThread();
		void run();

};

class ViAcoustidFingerprinter : public ViFingerprinter
{

	public:

		ViAcoustidFingerprinter();
		void generate(ViBufferOffsets bufferOffset);

};

#endif
