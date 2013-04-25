#ifndef VITARGETPROVIDER_H
#define VITARGETPROVIDER_H

#include <viserializer.h>


class ViTargetProvider : public ViSerializer
{

	public:

		ViTargetProvider();
		ViTargetProvider(const ViTargetProvider &other);
		virtual ~ViTargetProvider();

		virtual qreal calculate() = 0;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		virtual ViTargetProvider* clone() = 0;

};

#endif
