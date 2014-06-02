#ifndef VIVECTOR_H
#define VIVECTOR_H

#include <stdlib.h>
#include <QString>

//Adopted from Audacity - Dominic Mazzoni

class ViMatrix;

class ViVector
{

	public:

		ViVector();
		ViVector(const int &size, double *data = NULL, const bool &copy = true);
		ViVector(const int &size, const double *data);
		ViVector(const int &size1, const double *data1, const int &size2, const double *data2);
		ViVector(const ViVector &other);
		virtual ~ViVector();

		void clear();

		void resize(const int &size);
		void fill(const double &value);

		bool isValid() const; // at least 1 entry, with no entries being INF or NaN
		int size() const;
		double sum() const;

		static ViVector concatenate(const ViVector &left, const ViVector &right);
		ViVector& concatenate(const ViVector &other);

		ViVector subset(const int &start, const int &size) const;

		ViMatrix transpose();

		double& at(const int &index);
		double at(const int &index) const;

		double& operator [] (const int &index);
		double operator [] (const int &index) const;

		ViVector& operator = (const ViVector &other);

		ViVector& operator += (const ViVector &other);
		ViVector& operator -= (const ViVector &other);
		ViVector& operator *= (const ViVector &other);
		ViVector& operator *= (const double &value);
		ViVector& operator *= (const ViMatrix &matrix);

		ViVector operator + (const ViVector &other) const;
		ViVector operator - (const ViVector &other) const;
		ViVector operator * (const ViVector &other) const;
		ViVector operator * (const double &value) const;
		ViVector operator * (const ViMatrix &matrix) const;

		QString toString(const int &decimalPlaces = 5);

	private:

		void copy(const ViVector &other);

	private:

		int mSize;
		double *mData;
		bool mCopy;

};

#endif
