#ifndef VIFOURIERTRANSFORMER_H
#define VIFOURIERTRANSFORMER_H

#include "vifourierfixedthread.h"
#include "vifouriervariablethread.h"
#include "viwindowfunction.h"
#include "vicomplexnumber.h"

typedef QVector<ViComplexFloat> ViComplexVector;

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
			SeparateThread = 1
		};

	public:

		ViFourierTransformer(Execution execution = ViFourierTransformer::SameThread, int fixedSize = -1);
		~ViFourierTransformer();
		void emitFinished();

		bool setFixedSize(int size);
		void setExecution(Execution execution);
		
		void transform(float input[], float output[], ViWindowFunction<float> *windowFunction = NULL, Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output, ViWindowFunction<float> *windowFunction = NULL);
		void inverseTransform(float input[], float output[]);
		void rescale(float input[]);

		void transform(float input[], float output[], int numberOfSamples, ViWindowFunction<float> *windowFunction = NULL, Direction direction = ViFourierTransformer::Forward);
		void forwardTransform(float *input, float *output, int numberOfSamples, ViWindowFunction<float> *windowFunction = NULL);
		void inverseTransform(float input[], float output[], int numberOfSamples);

		void rescale(float input[], int numberOfSamples);
		static ViComplexVector toComplex(float input[], int numberOfSamples);

	private:

		void initialize();

		void fixedForwardTransform(float *input, float *output);
		void fixedInverseTransform(float input[], float output[]);
		void fixedRescale(float input[]);
		void variableForwardTransform(float input[], float output[], int numberOfSamples);
		void variableInverseTransform(float input[], float output[], int numberOfSamples);
		void variableRescale(float input[], int numberOfSamples);

		void forwardTransformSameThread();
		void forwardTransformSeperateThread();
		void inverseTransformSameThread();
		void inverseTransformSeperateThread();
		void rescaleTransformSameThread();
		void rescaleTransformSeperateThread();

	private:

		Execution mExecution;
		int mSize;

		QList<ViFourierThread*> mFixedForwardThreads;
		QList<ViFourierThread*> mFixedInverseThreads;
		QList<ViFourierThread*> mFixedRescaleThreads;
		ViFourierThread* mVariableForwardThread;
		ViFourierThread* mVariableInverseThread;
		ViFourierThread* mVariableRescaleThread;
		ViFourierThread *mForwardThread;
		ViFourierThread *mInverseThread;
		ViFourierThread *mRescaleThread;

		void (ViFourierTransformer::*forwardTransformtion)();
		void (ViFourierTransformer::*inverseTransformtion)();
		void (ViFourierTransformer::*rescaleTransformtion)();
};

#endif
