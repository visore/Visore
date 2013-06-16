#include <vicorrelator.h>
#include <float.h>

ViCorrelator::ViCorrelator()
    : ViDualProcessor()
{
    mMeanCorrelation = 0;
    mMinimumCorrelation = 0;
    mMaximumCorrelation = 0;
    mMeanTotal = 0;
    mCounter = 0;
}

ViCorrelator::~ViCorrelator()
{
}

void ViCorrelator::initialize()
{
    mMeanCorrelation = 0;
    mMinimumCorrelation = DBL_MAX;
    mMaximumCorrelation = -DBL_MAX;
    mMeanTotal = 0;
    mCounter = 0;
}

void ViCorrelator::finalize()
{
    mMeanTotal = 0;
    mCounter = 0;
}

qreal ViCorrelator::correlation(ViCorrelator::Type type)
{
    if(type == ViCorrelator::Mean)
    {
        return meanCorrelation();
    }
    else if(type == ViCorrelator::Minimum)
    {
        return minimumCorrelation();
    }
    else if(type == ViCorrelator::Maximum)
    {
        return maximumCorrelation();
    }
    else
    {
        return 0;
    }
}

qreal ViCorrelator::meanCorrelation()
{
    return mMeanCorrelation;
}

qreal ViCorrelator::minimumCorrelation()
{
    return mMinimumCorrelation;
}

qreal ViCorrelator::maximumCorrelation()
{
    return mMaximumCorrelation;
}

void ViCorrelator::addCorrelation(qreal mean)
{
    mMeanTotal += mean;
    ++mCounter;

    mMeanCorrelation = mMeanTotal / mCounter;

    if(mean < mMinimumCorrelation)
    {
        mMinimumCorrelation = mean;
    }
    else if(mean > mMaximumCorrelation)
    {
        mMaximumCorrelation = mean;
    }

    emit changed();
}

void ViCorrelator::setCorrelation(ViCorrelator::Type type, qreal correlation)
{
    if(type == ViCorrelator::Mean)
    {
        setMeanCorrelation(correlation);
    }
    else if(type == ViCorrelator::Minimum)
    {
        setMinimumCorrelation(correlation);
    }
    else if(type == ViCorrelator::Maximum)
    {
        setMaximumCorrelation(correlation);
    }
}

void ViCorrelator::setMeanCorrelation(qreal correlation)
{
    mMeanCorrelation = correlation;
}

void ViCorrelator::setMinimumCorrelation(qreal correlation)
{
    mMinimumCorrelation = correlation;
}

void ViCorrelator::setMaximumCorrelation(qreal correlation)
{
    mMaximumCorrelation = correlation;
}
