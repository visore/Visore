#ifndef VIERROR_H
#define VIERROR_H

#include <vichunk.h>
#include <QtGlobal>
#include <QMap>
#include <mpreal.h>

class ViErrorCollection;

class ViError
{

	friend class ViErrorCollection;

	public:

		static qreal calculateMse(const qreal *prediction, const qreal *observation, const int &size);
		static qreal calculateMse(const qreal *prediction1, const qreal *observation1, const int &size1, const qreal *prediction2, const qreal *observation2, const int &size2);

		ViError();

		void add(const qreal &prediction, const qreal &observation);
		void add(const qreal *prediction, const qreal *observation, const int &size);
		void add(const ViSampleChunk &prediction, const ViSampleChunk &observation);

		qreal mse() const;
		qreal rmse() const;
		qreal nrmse() const; // Normalized RMSE

		bool isValid() const;
		void clear();

	private:

		static mpfr::mpreal make(const qreal &value = 0);

	private:

		mpfr::mpreal mCount;
		mpfr::mpreal mTotal;
		qreal mMin;
		qreal mMax;

};

class ViErrorCollection
{

	public:

		// Returns the error at a certain index, instead of the size
		ViError& atIndex(const int &index);

		ViError& at(const int &size);

		ViError& operator[](const int &size);

		// Over all sizes
		qreal mse() const;
		qreal rmse() const;
		qreal nrmse() const;

		void clear();
		int size() const;

	private:

		QMap<int, ViError> mErrors;

};

#endif
