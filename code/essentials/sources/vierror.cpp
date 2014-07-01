#include <vierror.h>

#define PRECISION 512

ViError::ViError()
{
	clear();
}

void ViError::add(const qreal &prediction, const qreal &observation)
{
	if(prediction == DBL_MAX || observation == DBL_MAX || std::isnan(prediction) || std::isnan(observation))
	{
		mTotal += 1;
	}
	else
	{
		mTotal += mpfr::pow(prediction - observation, 2);
		if(observation < mMin) mMin = observation;
		else if(observation > mMax) mMax = observation;
	}
	++mCount;
}

void ViError::add(const qreal *prediction, const qreal *observation, const int &size)
{
	for(int i = 0; i < size; ++i)
	{
		add(prediction[i], observation[i]);
	}
}

void ViError::add(const ViSampleChunk &prediction, const ViSampleChunk &observation)
{
	for(int i = 0; i < prediction.size(); ++i)
	{
		add(prediction[i], observation[i]);
	}
}

qreal ViError::calculateMse(const qreal *prediction, const qreal *observation, const int &size)
{
	qreal result = 0;
	for(int i = 0; i < size; ++i) result += pow(prediction[i] - observation[i], 2);
	return result;
}

qreal ViError::calculateMse(const qreal *prediction1, const qreal *observation1, const int &size1, const qreal *prediction2, const qreal *observation2, const int &size2)
{
	int i;
	qreal result = 0;
	for(i = 0; i < size1; ++i) result += pow(prediction1[i] - observation1[i], 2);
	for(i = 0; i < size2; ++i) result += pow(prediction2[i] - observation2[i], 2);
	return result;
}

qreal ViError::mse() const
{
	if(isValid()) return (mTotal / mCount).toDouble();
	return -1;
}

qreal ViError::rmse() const
{
	if(isValid()) return mpfr::sqrt(mTotal / mCount).toDouble();
	return -1;
}

qreal ViError::nrmse() const
{
	if(isValid() && (mMax - mMin) != 0) return (mpfr::sqrt(mTotal / mCount) / (mMax - mMin)).toDouble();
	return -1;
}

mpfr::mpreal ViError::make(const qreal &value)
{
	return mpfr::mpreal(value, PRECISION);
}

bool ViError::isValid() const
{
	return mCount > 0;
}

void ViError::clear()
{
	mCount = make();
	mTotal = make();
	mMin = 1;
	mMax = -1;
}

ViError& ViErrorCollection::atIndex(const int &index)
{
	return mErrors[mErrors.keys()[index]];
}

ViError& ViErrorCollection::at(const int &size)
{
	if(!mErrors.contains(size)) mErrors.insert(size, ViError());
	return mErrors[size];
}

ViError& ViErrorCollection::operator[](const int &size)
{
	return at(size);
}

qreal ViErrorCollection::mse() const
{
	mpfr::mpreal count = ViError::make();
	mpfr::mpreal total = ViError::make();
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		if(mErrors[i].isValid())
		{
			count += mErrors[i].mCount;
			total += mErrors[i].mTotal;
		}
	}
	if(count == 0) return -1;
	return (total / count).toDouble();
}

qreal ViErrorCollection::rmse() const
{
	mpfr::mpreal count = ViError::make();
	mpfr::mpreal total = ViError::make();
	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		if(mErrors[i].isValid())
		{
			count += mErrors[i].mCount;
			total += mErrors[i].mTotal;
		}
	}
	if(count == 0) return -1;
	return mpfr::sqrt(total / count).toDouble();
}

qreal ViErrorCollection::nrmse() const
{
	mpfr::mpreal count = ViError::make();
	mpfr::mpreal total = ViError::make();
	qreal min = 1;
	qreal max = -1;

	QList<int> keys = mErrors.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		if(mErrors[i].isValid())
		{
			count += mErrors[i].mCount;
			total += mErrors[i].mTotal;
			if(mErrors[i].mMax > max) max = mErrors[i].mMax;
			else if(mErrors[i].mMin < min) min = mErrors[i].mMin;
		}
	}
	if(count == 0 || (max - min) == 0) return -1;
	return (mpfr::sqrt(total / count) / (max - min)).toDouble();
}

void ViErrorCollection::clear()
{
	mErrors.clear();
}

int ViErrorCollection::size() const
{
	return mErrors.size();
}
