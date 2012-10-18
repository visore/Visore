#ifndef VIPOINTER_H
#define VIPOINTER_H

#include "viatomicint.h"
#include "vifunctor.h"

template<class T>
class ViPointerData
{

	public:

		ViPointerData()
		{
		mData = NULL;
		mCounter = NULL;
		mLimiter = NULL;
		mDeleter = NULL;
		}

	//private:

		T* mData;
		ViAtomicInt *mCounter;
		ViAtomicInt *mLimiter;
		ViFunctor *mDeleter;

};

template<class T>
class ViPointer
{

	public:

		ViPointer();
		ViPointer(T *pointer);
		ViPointer(const ViPointer<T> &other);
		virtual ~ViPointer();

		void setDeleter(ViFunctor *deleter);
		void setUnusedLimit(int limit);

		int referenceCount();
		bool isUsed();
		bool isNull();

		T* data();
		T& operator * ();
		T* operator -> ();
		ViPointer<T>& operator = (const ViPointer<T> &other);

		bool operator == (const ViPointer<T> &other);
		bool operator == (const T &other);
		bool operator == (const T *other);
		bool operator != (const ViPointer<T> &other);
		bool operator != (const T &other);
		bool operator != (const T *other);

	protected:

		void destruct();

	private:

		ViPointerData<T> *mData;
		
};

//Template decleration and implementation should actually be in the same file
#include "vipointer.cpp"

#endif
