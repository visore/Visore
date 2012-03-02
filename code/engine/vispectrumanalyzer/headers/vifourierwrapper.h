#ifndef VIFOURIERWRAPPER_H
#define VIFOURIERWRAPPER_H

#include <QtGlobal>
#include "FFTRealFixLen.h"

#if defined Q_CC_MSVC
#    pragma warning(disable:4100)
#elif defined Q_CC_GNU
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined Q_CC_MWERKS
#    pragma warning off (10182)
#endif

// Each pass of the FFT processes 2^X samples, where X is the number below.
static const int FFT_POWER_OF_TWO = 12;

class ViFourierWrapperPrivate
{
	public:
		ffft::FFTRealFixLen<FFT_POWER_OF_TWO> mFourierTransform;
};

class ViFourierWrapper
{
	public:
		ViFourierWrapper();
		~ViFourierWrapper();

		typedef float DataType;
		void calculateFft(DataType in[], const DataType out[]);

	private:
		ViFourierWrapperPrivate*  mPrivate;
};

#endif
