#include <vicorrelation.h>
#include <float.h>

ViCorrelation::ViCorrelation()
{
    clear();
}

ViCorrelation::ViCorrelation(QString correlator)
{
    clear();
    mCorrelator = correlator;
}

ViCorrelation::ViCorrelation(QString correlator, ViAudio::Type type1, ViAudio::Type type2)
{
    clear();
    mCorrelator = correlator;
    mType1 = type1;
    mType2 = type2;
}

ViCorrelation::ViCorrelation(const ViCorrelation &other)
{
    mCorrelator = other.mCorrelator;
    mMean = other.mMean;
    mMinimum = other.mMinimum;
    mMaximum = other.mMaximum;
    mTotal = other.mTotal;
    mCounter = other.mCounter;
    mType1 = other.mType1;
    mType2 = other.mType2;
}

void ViCorrelation::clear()
{
    mCorrelator = "";
    mType1 = ViAudio::Undefined;
    mType2 = ViAudio::Undefined;
    mMean = 0;
    mMinimum = DBL_MAX;
    mMaximum = -DBL_MAX;
    mTotal = 0;
    mCounter = 0;
}

QString ViCorrelation::correlator()
{
    return mCorrelator;
}

void ViCorrelation::setCorrelator(QString correlator)
{
    mCorrelator = correlator;
}

ViAudio::Type ViCorrelation::type1()
{
    return mType1;
}

ViAudio::Type ViCorrelation::type2()
{
    return mType2;
}

void ViCorrelation::setTypes(ViAudio::Type type1, ViAudio::Type type2)
{
    setType1(type1);
    setType2(type2);
}

void ViCorrelation::setType1(ViAudio::Type type)
{
    mType1 = type;
}

void ViCorrelation::setType2(ViAudio::Type type)
{
    mType2 = type;
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
    return correlator() + " [" + viTypeToString(type1()) + ", " + viTypeToString(type2()) + "] (mean: " + QString::number(mean() * 100, 'f', 2) + "%, minimum: " + QString::number(minimum() * 100, 'f', 2) + "%, maximum: " + QString::number(maximum() * 100, 'f', 2) + "%)";
}

ViElement ViCorrelation::exportData()
{
    ViElement element("correlation");
    element.addChild("correlator", correlator());

    ViElement type("type");
    type.addChild("first", viTypeToString(type1()));
    type.addChild("second", viTypeToString(type2()));
    element.addChild(type);

    element.addChild("mean", mean());
    element.addChild("minimum", minimum());
    element.addChild("maximum", maximum());
    return element;
}

bool ViCorrelation::importData(ViElement element)
{
    if(element.name() == "correlation")
    {
        setCorrelator(element.child("correlator").toString());
        setMean(element.child("mean").toReal());
        setMinimum(element.child("minimum").toReal());
        setMaximum(element.child("maximum").toReal());

        ViElement type = element.child("type");
        setType1(viStringToType(type.child("first").toString()));
        setType2(viStringToType(type.child("second").toString()));

        return true;
    }
    return false;
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
    return  mCorrelator == other.mCorrelator &&
            mMean == other.mMean &&
            mMinimum == other.mMinimum &&
            mMaximum == other.mMaximum &&
            mType1 == other.mType1 &&
            mType2 == other.mType2;
}

bool ViCorrelation::operator !=(const ViCorrelation &other)
{
    return !(*this == other);
}
