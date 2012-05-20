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

		ViFourierTransformer(Execution execution = ViFourierTransformer::SameThread, int fixedSize = -1);
		~ViFourierTransformer();

		bool setFixedSize(int size);
		void setExecution(Execution execution);
		
		void transform(float input[], float output[], ViWindower *windower = NULL, Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output, ViWindower *windower = NULL);
		void inverseTransform(float input[], float output[]);

		void transform(float input[], float output[], qint32 numberOfSamples, ViWindower *windower = NULL, Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output, qint32 numberOfSamples, ViWindower *windower = NULL);
		void inverseTransform(float input[], float output[], qint32 numberOfSamples);

	private:

		void initialize();

		void fixedForwardTransform(float *input, float *output);
		void fixedInverseTransform(float input[], float output[]);
		void variableForwardTransform(float input[], float output[], qint32 numberOfSamples);
		void variableInverseTransform(float input[], float output[], qint32 numberOfSamples);

		void forwardTransformSameThread();
		void forwardTransformSeperateThread();
		void inverseTransformSameThread();
		void inverseTransformSeperateThread();

	private:

		Execution mExecution;
		int mSize;

		QList<ViFourierThread*> mFixedForwardThreads;
		QList<ViFourierThread*> mFixedInverseThreads;
		ViFourierThread* mVariableForwardThread;
		ViFourierThread* mVariableInverseThread;
		ViFourierThread *mForwardThread;
		ViFourierThread *mInverseThread;

		void (ViFourierTransformer::*forwardTransformtion)();
		void (ViFourierTransformer::*inverseTransformtion)();
};

#endif
