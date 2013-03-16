#ifndef VIACTIVATIONFUNCTIONTYPE_H
#define VIACTIVATIONFUNCTIONTYPE_H

#include <viactivationfunction.h>
#include <vitype.h>

/*
	Note: Subclasses must inheit from ViActivationFunctionType
*/

template <typename T>
class ViActivationFunctionType : public ViActivationFunction, public ViType<T>
{

	public:

	using ViActivationFunction::ViActivationFunction; // C++11 standard, inherit constructors

	/*public:

		ViActivationFunctionType(double functionMinimum = 0, double functionMaximum = 1);
		ViActivationFunctionType(const ViActivationFunctionType<T> &other);
		virtual ~ViActivationFunctionType();*/
};

#include <viactivationfunctiontype.cpp>

#endif
