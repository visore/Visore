#ifndef VIFUNCTOR_H
#define VIFUNCTOR_H

class ViFunctorParameter
{
	virtual void dummy(){} // Needed for dynamic_cast
};

class ViFunctor
{

	public:

		virtual void execute(ViFunctorParameter *data = 0) = 0;
		virtual void execute(void *data = 0);
		virtual void operator () (ViFunctorParameter *data = 0);

};

#endif
