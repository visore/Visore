#ifndef VIFIXEDMODELORDER_H
#define VIFIXEDMODELORDER_H

#include <vimodelorder.h>

class ViFixedModelOrder : public ViModelOrder
{

	public:

		ViFixedModelOrder();
		ViFixedModelOrder(const ViFixedModelOrder &other);
		virtual ~ViFixedModelOrder();

		ViFixedModelOrder* clone();

		void clear();

		void setOrder(const int &order);

		int order() const;

	protected:

		int mOrder;

};

#endif
