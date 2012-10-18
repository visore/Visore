#ifdef VIFUNCTOR_H

template<class T, class P>
ViFunctorObject<T, P>::ViFunctorObject()
{
	mObject = NULL;
	mFunction = NULL;
}

template<class T, class P>
ViFunctorObject<T, P>::ViFunctorObject(T *object, ViFunction function)
{
	mObject = object;
	mFunction = function;
}

template<class T, class P>
ViFunctorObject<T, P>::ViFunctorObject(const ViFunctorObject &other)
{
	mObject = other.mObject;
	mFunction = other.mFunction;
}

template<class T, class P>
void ViFunctorObject<T, P>::execute(void *data)
{
	if(mObject != NULL)
	{
		(((T*)mObject)->*mFunction)((P*) data);
	}
}

#endif
