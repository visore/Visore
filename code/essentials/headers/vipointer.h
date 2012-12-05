#ifndef VIPOINTER_H
#define VIPOINTER_H

#include "viatomicint.h"
#include "vifunctor.h"

template<class T>
class ViPointerData
{

	public:

		ViPointerData(T *data = NULL, ViFunctor *deleter = NULL, int counter = 1, int limiter = 0);
		
		void setData(T *data);
		void setDeleter(ViFunctor *deleter);
		void setCounter(int counter);
		void setLimiter(int limiter);
		
		T* data();
		ViFunctor* deleter();
		ViAtomicInt& counter();
		ViAtomicInt& limiter();

	private:

		T *mData;
		ViFunctor *mDeleter;
		ViAtomicInt mCounter;
		ViAtomicInt mLimiter;	

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
		void setNull();

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

		void copy(const ViPointer<T> &other);
		void destruct();

	private:

		ViPointerData<T> *mData;
		
};

//Template decleration and implementation should actually be in the same file
#include "vipointer.cpp"

#endif
