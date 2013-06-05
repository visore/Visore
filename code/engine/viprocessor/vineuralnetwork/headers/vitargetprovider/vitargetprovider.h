#ifndef VITARGETPROVIDER_H
#define VITARGETPROVIDER_H

#include <viserializer.h>
#include <vilibrary.h>
#include <vichunk.h>

class ViTargetProvider : public ViLibrary, public ViSerializer
{

	public:

		ViTargetProvider(int leftSamples = 0, int rightSamples = 0);
		ViTargetProvider(const ViTargetProvider &other);
		virtual ~ViTargetProvider();

		virtual int leftSamples();
		virtual int rightSamples();
		virtual void setLeftSamples(int samples);
		virtual void setRightSamples(int samples);

		virtual void setData(ViSampleChunk *left, ViSampleChunk *right);
		virtual void setLeftData(ViSampleChunk *data);
		virtual void setRightData(ViSampleChunk *data);

		virtual qreal calculate() = 0;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViTargetProvider* clone() = 0;

	protected:

		ViSampleChunk *mLeftData;
		ViSampleChunk *mRightData;

		int mLeftSamples;
		int mRightSamples;

};

#endif
