#ifndef VIMATRIX_H
#define VIMATRIX_H

#include <vivector.h>

//Adopted from Audacity - Dominic Mazzoni

class ViMatrix
{

	public:

		ViMatrix();
		ViMatrix(const int &rows, const int &colums, const double **data = NULL);
		ViMatrix(const ViMatrix &other);
		virtual ~ViMatrix();

		void clear();

		bool isValid() const; //at least 1 column and row
		int rows() const;
		int columns() const;
		double sum() const;

		ViMatrix invert();
		ViMatrix invert(bool &success);
		bool invert(ViMatrix &inverted);

		ViMatrix transpose() const;

		void swapRows(const int &first, const int &second);

		ViMatrix scalarMultiply(const ViMatrix &matrix) const;
		ViMatrix matrixMultiply(const ViMatrix &matrix) const;

		ViMatrix subset(const int &startRow, const int &rowCount, const int &startColumn, const int &columnCount) const;

		ViMatrix concatenateColums(const ViMatrix &matrix) const;

		static ViMatrix identityMatrix(const int &size);

		ViMatrix& operator = (const ViMatrix& other);

		ViMatrix operator + (const ViMatrix &matrix) const;
		ViMatrix operator * (const double &value) const;
		ViVector operator * (const ViVector &vector) const;
		ViMatrix operator * (const ViMatrix &matrix) const; // Scalar multiplication

		ViVector& operator [] (const int &index);
		ViVector& operator [] (const int &index) const;

		QString toString();

	private:

		void copy(const ViMatrix& other);

	private:

		int mRows;
		int mColumns;
		ViVector **mVectors;

};

#endif
