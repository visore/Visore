#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include <QtGlobal>
#include <QThread>
#include "FFTRealFixLen.h"

// Each pass of the FFT processes 2^X samples, where X is the number below.
static const int FFT_POWER_OF_TWO = 12;

class ViFourierObject : public QObject
{
	Q_OBJECT

	signals:

		void finished();
};

template <typename T>
class ViFourierTransformerThread : public QThread
{

	public:

		ViFourierTransformerThread();
		void setData(T input[], T output[]);

	protected:

		ffft::FFTRealFixLen<FFT_POWER_OF_TWO> mFourierTransform;
		T *mInput;
		T *mOutput;

};

template <typename T>
class ViFourierTransformerForwardThread : public ViFourierTransformerThread<T>
{

	public:

		ViFourierTransformerForwardThread();
		void run();

};

template <typename T>
class ViFourierTransformerInverseThread : public ViFourierTransformerThread<T>
{

	public:

		ViFourierTransformerInverseThread();
		void run();

};

template <typename T>
class ViFourierTransformer : public ViFourierObject
{

	public:

		enum Direction
		{
			Forward = 0,
			Inverse = 1
		};

	public:

		ViFourierTransformer();
		
		void transform(T input[], T output[], Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(T input[], T output[]);
		void inverseTransform(T input[], T output[]);

	private:

		ViFourierTransformerForwardThread<T>  mForwardThread;
		ViFourierTransformerInverseThread<T>  mInverseThread;
};

#include "../sources/vifouriertransformer.cpp"

#endif
