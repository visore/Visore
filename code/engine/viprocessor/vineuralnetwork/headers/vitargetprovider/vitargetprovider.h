#ifndef VITARGETPROVIDER_H
#define VITARGETPROVIDER_H

#include <viserializer.h>
#include <vilibrary.h>
#include <vichunk.h>

class ViTargetProvider : public ViLibrary, public ViSerializer
{

	public:

		ViTargetProvider();
		ViTargetProvider(const ViTargetProvider &other);
		virtual ~ViTargetProvider();

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

};

#endif
