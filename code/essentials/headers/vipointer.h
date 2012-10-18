#ifndef VIPOINTER_H
#define VIPOINTER_H

#include "viatomicint.h"
/*
#include <QSharedPointer>

template<class T>
class ViPointer : public QSharedPointer<T>
{

	public:


		typedef void (*Deleter)(T*);

	public:

		ViPointer();
		ViPointer(T *pointer);
		ViPointer(T *pointer, Deleter deleter);
		ViPointer(const ViPointer<T> &other);
		ViPointer(const QSharedPointer<T> &other);
		ViPointer(const QWeakPointer<T> &other);
		virtual ~ViPointer();

		int referenceCount();
		bool isUsed();

		ViPointer<T>& operator = (const QSharedPointer<T> &other);

	private:

		ViAtomicInt *mCounter;
		
};
*/

#include "vifunctor.h"

template<class T>
class ViPointer
{

	public:

		ViPointer();
		ViPointer(T *pointer);
		ViPointer(const ViPointer<T> &other);
		virtual ~ViPointer();

		void setDeleter(ViFunctor deleter){mDeleter = deleter;}

		int referenceCount();
		bool isUsed();
		bool isNull();

		T* data();
		T& operator * ();
		T* operator -> ();

	private:

		T* mData;
		ViAtomicInt *mCounter;
		ViAtomicInt *mLimiter;
		ViFunctor mDeleter;
		
};

//Template decleration and implementation should actually be in the same file
#include "../sources/vipointer.cpp"

#endif
