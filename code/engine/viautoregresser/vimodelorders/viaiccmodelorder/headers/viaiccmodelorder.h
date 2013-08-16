#ifndef VIAICCMODELORDER_H
#define VIAICCMODELORDER_H

#include <viaicmodelorder.h>

class ViAiccModelOrder : public ViAicModelOrder
{

	public:

		ViAiccModelOrder();
		ViAiccModelOrder(const ViAiccModelOrder &other);
		virtual ~ViAiccModelOrder();

		ViAiccModelOrder* clone();

		static int order(const int &sampleCount, const int &parameterCount, const qreal &rss);

};

#endif
