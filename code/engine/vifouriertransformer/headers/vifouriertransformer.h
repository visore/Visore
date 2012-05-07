#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include <QtGlobal>
#include <QThread>
#include <QtCore/qmath.h>
#include "FFTRealFixLen.h"
#include "vifourierfixedthread.h"
#include "vifouriervariablethread.h"

class ViFourierObject : public QObject
{
	Q_OBJECT

	signals:

		void finished();
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
		
		void transform(T input[], T output[], Direction direction = ViFourierTransformer::Forward, qint32 numberOfSamples = FFT_POWER_OF_TWO);
		void forwardTransform(T input[], T output[], qint32 numberOfSamples = FFT_POWER_OF_TWO);
		void inverseTransform(T input[], T output[], qint32 numberOfSamples = FFT_POWER_OF_TWO);

	private:

		void fixedForwardTransform(T input[], T output[]);
		void fixedInverseTransform(T input[], T output[]);
		void variableForwardTransform(T input[], T output[], qint32 numberOfSamples);
		void variableInverseTransform(T input[], T output[], qint32 numberOfSamples);

	private:

		ViFourierFixedForwardThread<T>  mFixedForwardThread;
		ViFourierFixedInverseThread<T>  mFixedInverseThread;
		ViFourierVariableForwardThread<T>  mVariableForwardThread;
		ViFourierVariableInverseThread<T>  mVariableInverseThread;
};

#include "../sources/vifouriertransformer.cpp"

#endif
