#ifndef VIFUNCTOR_H
#define VIFUNCTOR_H

class ViFunctorParameter
{
};

class ViFunctor
{

	public:

		virtual void execute (ViFunctorParameter *data = 0) = 0;
		virtual void operator () (ViFunctorParameter *data = 0);

};

#endif
