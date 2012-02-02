#ifndef VIFOURIERWRAPPER_H
#define VIFOURIERWRAPPER_H

#include <QtGlobal>
#include "FFTRealFixLen.h"

#if defined(FFTREAL_LIBRARY)
#  define FFTREAL_EXPORT Q_DECL_EXPORT
#else
#  define FFTREAL_EXPORT Q_DECL_IMPORT
#endif

#if defined Q_CC_MSVC
#    pragma warning(disable:4100)
#elif defined Q_CC_GNU
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined Q_CC_MWERKS
#    pragma warning off (10182)
#endif

// Each pass of the FFT processes 2^X samples, where X is the number below.
static const int FFTLengthPowerOfTwo = 12;

class ViFourierWrapperPrivate
{
	public:
		ffft::FFTRealFixLen<FFTLengthPowerOfTwo> mFourierTransform;
};

class FFTREAL_EXPORT ViFourierWrapper
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
