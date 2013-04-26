#ifndef VITARGETPROVIDER_H
#define VITARGETPROVIDER_H

#include <viserializer.h>
#include <vichunk.h>

class ViTargetProvider : public ViSerializer
{

	public:

		ViTargetProvider();
		ViTargetProvider(const ViTargetProvider &other);
		virtual ~ViTargetProvider();

		virtual void setData(ViSampleChunk chunk);
		virtual const ViSampleChunk& data();

		virtual qreal calculate(int index = -1) = 0;
		virtual ViRealList calculate(ViRealList indexes);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViTargetProvider* clone() = 0;

	private:

		ViSampleChunk mData;

};

#endif
