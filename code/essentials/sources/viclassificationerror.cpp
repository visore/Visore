#include <viclassificationerror.h>

qint64 ViClassificationError::calculateTP(const bool *classification, const bool *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateTP(const int *classification, const int *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateTN(const bool *classification, const bool *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(!classification[i] && !observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateTN(const int *classification, const int *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(!classification[i] && !observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateFP(const bool *classification, const bool *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && !observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateFP(const int *classification, const int *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && !observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateFN(const bool *classification, const bool *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(!classification[i] && observation[i]) ++count;
	}
	return count;
}

qint64 ViClassificationError::calculateFN(const int *classification, const int *observation, const int &size)
{
	qint64 count = 0;
	for(int i = 0; i < size; ++i)
	{
		if(!classification[i] && observation[i]) ++count;
	}
	return count;
}

qreal ViClassificationError::calculateSensitivity(const qint64 &TP, const qint64 &FN)
{
	return qreal(TP) / (TP + FN);
}

qreal ViClassificationError::calculateSensitivity(const bool *classification, const bool *observation, const int &size)
{
	return calculateSensitivity(calculateTP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateSensitivity(const int *classification, const int *observation, const int &size)
{
	return calculateSensitivity(calculateTP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateSpecificity(const qint64 &TN, const qint64 &FP)
{
		return qreal(TN) / (TN + FP);
}

qreal ViClassificationError::calculateSpecificity(const bool *classification, const bool *observation, const int &size)
{
	return calculateSpecificity(calculateTN(classification, observation, size), calculateFP(classification, observation, size));
}

qreal ViClassificationError::calculateSpecificity(const int *classification, const int *observation, const int &size)
{
	return calculateSpecificity(calculateTN(classification, observation, size), calculateFP(classification, observation, size));
}

qreal ViClassificationError::calculateAccuracy(const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN)
{
	return qreal(TP + TN) / (TP + TN + FP + FN);
}

qreal ViClassificationError::calculateAccuracy(const bool *classification, const bool *observation, const int &size)
{
	return calculateAccuracy(calculateTP(classification, observation, size), calculateTN(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateAccuracy(const int *classification, const int *observation, const int &size)
{
	return calculateAccuracy(calculateTP(classification, observation, size), calculateTN(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateF1Score(const qint64 &TP, const qint64 &FP, const qint64 &FN)
{
	return qreal(2 * TP) / ((2 * TP) + FP + FN);
}

qreal ViClassificationError::calculateF1Score(const bool *classification, const bool *observation, const int &size)
{
	return calculateF1Score(calculateTP(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateF1Score(const int *classification, const int *observation, const int &size)
{
	return calculateF1Score(calculateTP(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateMatthewsCoefficient(const qint64 &TP, const qint64 &TN, const qint64 &FP, const qint64 &FN)
{
	return ((TP * TN) - (FP * FN)) / (sqrt(TP + FP) * sqrt(TP + FN) * sqrt(TN + FP) * sqrt(TN + FN)); // Take individual sqrt, otherwise the number gets to large with multiplication
}

qreal ViClassificationError::calculateMatthewsCoefficient(const bool *classification, const bool *observation, const int &size)
{
	return calculateMatthewsCoefficient(calculateTP(classification, observation, size), calculateTN(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

qreal ViClassificationError::calculateMatthewsCoefficient(const int *classification, const int *observation, const int &size)
{
	return calculateMatthewsCoefficient(calculateTP(classification, observation, size), calculateTN(classification, observation, size), calculateFP(classification, observation, size), calculateFN(classification, observation, size));
}

ViClassificationError::ViClassificationError()
{
	clear();
}

ViClassificationError::ViClassificationError(const ViClassificationError &other)
{
	mTP = other.mTP;
	mTN = other.mTN;
	mFP = other.mFP;
	mFN = other.mFN;
}

void ViClassificationError::clear()
{
	mTP = 0;
	mTN = 0;
	mFP = 0;
	mFN = 0;
}

void ViClassificationError::addTP()
{
	++mTP;
}

void ViClassificationError::addTN()
{
	++mTN;
}

void ViClassificationError::addFP()
{
	++mFP;
}

void ViClassificationError::addFN()
{
	++mFN;
}

void ViClassificationError::add(const bool &classification, const bool &observation)
{
	if(classification && observation) ++mTP;
	else if(!classification && !observation) ++mTN;
	else if(classification && !observation) ++mFP;
	else if(!classification && observation) ++mFN;
}

void ViClassificationError::add(const int &classification, const int &observation)
{
	if(classification && observation) ++mTP;
	else if(!classification && !observation) ++mTN;
	else if(classification && !observation) ++mFP;
	else if(!classification && observation) ++mFN;
}

void ViClassificationError::add(const bool *classification, const bool *observation, const int &size)
{
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && observation[i]) ++mTP;
		else if(!classification[i] && !observation[i]) ++mTN;
		else if(classification[i] && !observation[i]) ++mFP;
		else if(!classification[i] && observation[i]) ++mFN;
	}
}

void ViClassificationError::add(const int *classification, const int *observation, const int &size)
{
	for(int i = 0; i < size; ++i)
	{
		if(classification[i] && observation[i]) ++mTP;
		else if(!classification[i] && !observation[i]) ++mTN;
		else if(classification[i] && !observation[i]) ++mFP;
		else if(!classification[i] && observation[i]) ++mFN;
	}
}

qint64 ViClassificationError::TP() const
{
	return mTP;
}

qint64 ViClassificationError::TN() const
{
	return mTN;
}

qint64 ViClassificationError::FP() const
{
	return mFP;
}

qint64 ViClassificationError::FN() const
{
	return mFN;
}

qreal ViClassificationError::sensitivity() const
{
	return calculateSensitivity(mTP, mFN);
}

qreal ViClassificationError::specificity() const
{
	return calculateSpecificity(mTN, mFP);
}

qreal ViClassificationError::accuracy() const
{
	return calculateAccuracy(mTP, mTN, mFP, mFN);
}

qreal ViClassificationError::f1Score() const
{
	return calculateF1Score(mTP, mFP, mFN);
}

qreal ViClassificationError::matthewsCoefficient() const
{
	return calculateMatthewsCoefficient(mTP, mTN, mFP, mFN);
}


ViClassificationError& ViClassificationErrorCollection::atIndex(const int &index)
{
	return mErrors[mErrors.keys()[index]];
}

ViClassificationError& ViClassificationErrorCollection::at(const int &size)
{
	if(!mErrors.contains(size)) mErrors.insert(size, ViClassificationError());
	return mErrors[size];
}

ViClassificationError& ViClassificationErrorCollection::operator[](const int &size)
{
	return at(size);
}

void ViClassificationErrorCollection::clear()
{
	mErrors.clear();
}

qint64 ViClassificationErrorCollection::TP() const
{
	qint64 total = 0;
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		total += mErrors[i].TP();
	}
	return total;
}

qint64 ViClassificationErrorCollection::TN() const
{
	qint64 total = 0;
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		total += mErrors[i].TN();
	}
	return total;
}

qint64 ViClassificationErrorCollection::FP() const
{
	qint64 total = 0;
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		total += mErrors[i].FP();
	}
	return total;
}

qint64 ViClassificationErrorCollection::FN() const
{
	qint64 total = 0;
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		total += mErrors[i].FN();
	}
	return total;
}

qreal ViClassificationErrorCollection::sensitivity() const
{
	return ViClassificationError::calculateSensitivity(TP(), FN());
}

qreal ViClassificationErrorCollection::specificity() const
{
	return ViClassificationError::calculateSpecificity(TN(), FP());
}

qreal ViClassificationErrorCollection::accuracy() const
{
	return ViClassificationError::calculateAccuracy(TP(), TN(), FP(), FN());
}

qreal ViClassificationErrorCollection::f1Score() const
{
	return ViClassificationError::calculateF1Score(TP(), FP(), FN());
}

qreal ViClassificationErrorCollection::matthewsCoefficient() const
{
	return ViClassificationError::calculateMatthewsCoefficient(TP(), TN(), FP(), FN());
}
