#ifndef VISINGLETON_H
#define VISINGLETON_H

#include <vipointer.h>

template<typename T>
class ViSingleton
{

	public:

		static ViPointer<T> instance();

	protected:

		static ViPointer<T> mInstance;

};

#include <visingleton.cpp>

#endif
