#ifndef VIFUNCTOR_H
#define VIFUNCTOR_H
#include "vilogger.h"
class ViFunctor
{

	public:

		virtual void execute(void *data = NULL){cout<<"poppopopo: "<<typeid(this).name()<<endl;}
		virtual void operator () (void *data = NULL){ execute(data); }

};

template<class T, class P>
class ViFunctorObject : public ViFunctor
{

	public:

		typedef void (T::*ViFunction)(P*);

	public:

		ViFunctorObject();
		ViFunctorObject(T *object, ViFunction function);
		ViFunctorObject(const ViFunctorObject &other);
		virtual void execute(void *data = NULL);

	private:

		T *mObject;
		ViFunction mFunction;

};

#include "vifunctor.cpp"

#endif
