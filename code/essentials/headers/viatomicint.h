#ifndef VIATOMICINT_H
#define VIATOMICINT_H

#include <QMutex>
#include <QMutexLocker>

class ViAtomicInt
{

	public:

		ViAtomicInt(int value = 0);

		void increase();
		void decrease();

		int value();
		void setValue(int value);

		//int operator ++ (const int value);
		//int operator -- (const int value);

	private:

		int mValue;
		QMutex mMutex;
		
};

#endif
