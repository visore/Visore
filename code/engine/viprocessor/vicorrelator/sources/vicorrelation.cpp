#include <vicorrelation.h>
#include <float.h>

ViCorrelation::ViCorrelation()
{
    clear();
}

ViCorrelation::ViCorrelation(const ViCorrelation &other)
{
    mMean = other.mMean;
    mMinimum = other.mMinimum;
    mMaximum = other.mMaximum;
    mTotal = other.mTotal;
    mCounter = other.mCounter;
}

void ViCorrelation::clear()
{
    mMean = 0;
    mMinimum = DBL_MAX;
    mMaximum = -DBL_MAX;
    mTotal = 0;
    mCounter = 0;
}

bool ViCorrelation::isValid()
{
    return mMean != 0 && mMinimum != DBL_MAX && mMaximum != -DBL_MAX;
}

qreal ViCorrelation::correlation(ViCorrelation::Type type)
{
    if(type == ViCorrelation::Mean) return mean();
    else if(type == ViCorrelation::Minimum) return minimum();
    else if(type == ViCorrelation::Maximum) return maximum();
    else return 0;
}

qreal ViCorrelation::mean()
{
    return mMean;
}

qreal ViCorrelation::minimum()
{
    return mMinimum;
}

qreal ViCorrelation::maximum()
{
    return mMaximum;
}

void ViCorrelation::addCorrelation(qreal value)
{
    addCorrelation(value, value, value);
}

void ViCorrelation::addCorrelation(qreal mean, qreal minimum, qreal maximum)
{
    addMean(mean);
    if(minimum < mMinimum) mMinimum = minimum;
    else if(maximum > mMaximum) mMaximum = maximum;
}

void ViCorrelation::addMean(qreal mean)
{
    mTotal += mean;
    ++mCounter;
    mMean = mTotal / mCounter;
}

void ViCorrelation::setCorrelation(ViCorrelation::Type type, qreal correlation)
{
    if(type == ViCorrelation::Mean) setMean(correlation);
    else if(type == ViCorrelation::Minimum) setMinimum(correlation);
    else if(type == ViCorrelation::Maximum) setMaximum(correlation);
}

void ViCorrelation::setMean(qreal correlation)
{
    mMean = correlation;
}

void ViCorrelation::setMinimum(qreal correlation)
{
    mMinimum = correlation;
}

void ViCorrelation::setMaximum(qreal correlation)
{
    mMaximum = correlation;
}

QString ViCorrelation::toString()
{
    return "mean: " + QString::number(mean() * 100, 'f', 2) + "%, minimum: " + QString::number(minimum() * 100, 'f', 2) + "%, maximum: " + QString::number(maximum() * 100, 'f', 2) + "%";
}

ViElement ViCorrelation::exportData()
{
    ViElement element("correlation");
    element.addChild("mean", mean());
    element.addChild("minimum", minimum());
    element.addChild("maximum", maximum());
    return element;
}

bool ViCorrelation::importData(ViElement element)
{
    if(element.name() != "correlation")
    {
        if(element.hasChild("correlation")) element = element.child("correlation");
        else return false;
    }

    setMean(element.child("mean").toReal());
    setMinimum(element.child("minimum").toReal());
    setMaximum(element.child("maximum").toReal());

    return true;
}

bool ViCorrelation::operator >(const ViCorrelation &other)
{
    return mMean > other.mMean;
}

bool ViCorrelation::operator <(const ViCorrelation &other)
{
    return mMean < other.mMean;
}

bool ViCorrelation::operator ==(const ViCorrelation &other)
{
    return  mMean == other.mMean &&
            mMinimum == other.mMinimum &&
            mMaximum == other.mMaximum;
}

bool ViCorrelation::operator !=(const ViCorrelation &other)
{
    return !(*this == other);
}
