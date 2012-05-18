#ifndef VIFREQUENCYSPECTRUM_H
#define VIFREQUENCYSPECTRUM_H

#include <QMutex>
#include <QVector>
#include "vicomplexnumber.h"

class ViFrequencySpectrum
{

	public:

		qreal real(const qint32 index);
		qreal imaginary(const qint32 index);
		ViComplexFloat at(const qint32 index);
		ViComplexFloat operator[](const qint32 index) const;

		void append(ViComplexFloat complex);
		void setReal(const qint32 index, qreal real);
		void setImaginary(const qint32 index, qreal imaginary);
		void set(const qint32 index, qreal real, qreal imaginary);
		void set(const qint32 index, ViComplexFloat complex);
		ViComplexFloat& operator[](const qint32 index);

		qint32 size();

		void lock();
		void unlock();

	private:

		QVector<ViComplexFloat> mData;
		QMutex mMutex;

};

#endif
