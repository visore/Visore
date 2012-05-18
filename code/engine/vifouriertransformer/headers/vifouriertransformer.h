#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include "vifourierfixedthread.h"
#include "vifouriervariablethread.h"
#include "viwindower.h"

class ViFourierTransformer : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	public:

		enum Direction
		{
			Forward = 0,
			Inverse = 1
		};

		enum Execution
		{
			SameThread = 0,
			SeperateThread = 1
		};

	public:

		ViFourierTransformer(Execution execution = ViFourierTransformer::SameThread);

		qint32 optimalSize();
		
		void transform(float input[], float output[], qint32 numberOfSamples, ViWindower *windower = NULL, Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output, qint32 numberOfSamples, ViWindower *windower = NULL);
		void inverseTransform(float input[], float output[], qint32 numberOfSamples);

	private:

		void fixedForwardTransform(float *input, float *output);
		void fixedInverseTransform(float input[], float output[]);
		void variableForwardTransform(float input[], float output[], qint32 numberOfSamples);
		void variableInverseTransform(float input[], float output[], qint32 numberOfSamples);

	private:

		Execution mExecution;
		ViFourierFixedForwardThread mFixedForwardThread;
		ViFourierFixedInverseThread mFixedInverseThread;
		ViFourierVariableForwardThread mVariableForwardThread;
		ViFourierVariableInverseThread mVariableInverseThread;
};

#endif
