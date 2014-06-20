#ifndef VICLASSIFICATIONERROR_H
#define VICLASSIFICATIONERROR_H

#include <QtGlobal>
#include <QMap>
#include <mpreal.h>

class ViClassificationErrorCollection;

class ViClassificationError
{

	public:

		static qint64 calculateTP(const bool *classification, const bool *observation, const int &size);
		static qint64 calculateTP(const int *classification, const int *observation, const int &size);
		static qint64 calculateTN(const bool *classification, const bool *observation, const int &size);
		static qint64 calculateTN(const int *classification, const int *observation, const int &size);
		static qint64 calculateFP(const bool *classification, const bool *observation, const int &size);
		static qint64 calculateFP(const int *classification, const int *observation, const int &size);
		static qint64 calculateFN(const bool *classification, const bool *observation, const int &size);
		static qint64 calculateFN(const int *classification, const int *observation, const int &size);

		static qreal calculateSensitivity(const qint64 &TP, const qint64 &FN);
		static qreal calculateSensitivity(const bool *classification, const bool *observation, const int &size);
		static qreal calculateSensitivity(const int *classification, const int *observation, const int &size);

		static qreal calculateSpecificity(const qint64 &TN, const qint64 &FP);
		static qreal calculateSpecificity(const bool *classification, const bool *observation, const int &size);
		static qreal calculateSpecificity(const int *classification, const int *observation, const int &size);

		static qreal calculateAccuracy(const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN);
		static qreal calculateAccuracy(const bool *classification, const bool *observation, const int &size);
		static qreal calculateAccuracy(const int *classification, const int *observation, const int &size);

		static qreal calculateF1Score(const qint64 &TP, const qint64 &FP, const qint64 &FN);
		static qreal calculateF1Score(const bool *classification, const bool *observation, const int &size);
		static qreal calculateF1Score(const int *classification, const int *observation, const int &size);

		static qreal calculateMatthewsCoefficient(const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN);
		static qreal calculateMatthewsCoefficient(const bool *classification, const bool *observation, const int &size);
		static qreal calculateMatthewsCoefficient(const int *classification, const int *observation, const int &size);

	public:

		ViClassificationError();
		ViClassificationError(const ViClassificationError &other);

		void clear();

		void addTP();
		void addTN();
		void addFP();
		void addFN();
		void add(const bool &classification, const bool &observation);
		void add(const int &classification, const int &observation);
		void add(const bool *classification, const bool *observation, const int &size);
		void add(const int *classification, const int *observation, const int &size);

		qint64 TP() const;
		qint64 TN() const;
		qint64 FP() const;
		qint64 FN() const;

		qreal sensitivity() const;
		qreal specificity() const;
		qreal accuracy() const;
		qreal f1Score() const;
		qreal matthewsCoefficient() const;

	private:

		qint64 mTP;
		qint64 mTN;
		qint64 mFP;
		qint64 mFN;

};

class ViClassificationErrorCollection
{

	public:

		// Returns the error at a certain index, instead of the size
		ViClassificationError& atIndex(const int &index);

		ViClassificationError& at(const int &size);

		ViClassificationError& operator[](const int &size);

		void clear();

		qint64 TP() const;
		qint64 TN() const;
		qint64 FP() const;
		qint64 FN() const;

		qreal sensitivity() const;
		qreal specificity() const;
		qreal accuracy() const;
		qreal f1Score() const;
		qreal matthewsCoefficient() const;

	private:

		QMap<int, ViClassificationError> mErrors;

};

#endif
