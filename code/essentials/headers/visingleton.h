#ifndef VISINGELTON_H
#define VISINGELTON_H

#include <QSharedPointer>

template<typename T>
class ViSingleton
{

	public:

		static QSharedPointer<T> instance();

	protected:

		static QSharedPointer<T> mInstance;

};

#include "visingleton.cpp"

#endif
