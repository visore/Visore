#include <vicorrelationgroup.h>
#include <vicorrelator.h>
#include <vilogger.h>

ViCorrelationGroup::ViCorrelationGroup()
{
    clear();
}

ViCorrelationGroup::ViCorrelationGroup(ViAudioTypePair pair)
{
    clear();
    setTypes(pair.first, pair.second);
}

ViCorrelationGroup::ViCorrelationGroup(ViAudio::Type type1, ViAudio::Type type2)
{
    clear();
    setTypes(type1, type2);
}

ViCorrelationGroup::ViCorrelationGroup(const ViCorrelationGroup &other)
{
    mType1 = other.mType1;
    mType2 = other.mType2;
    mCorrelations = other.mCorrelations;
}

ViAudio::Type ViCorrelationGroup::type1()
{
    return mType1;
}

ViAudio::Type ViCorrelationGroup::type2()
{
    return mType2;
}

void ViCorrelationGroup::setTypes(ViAudio::Type type1, ViAudio::Type type2)
{
    setType1(type1);
    setType2(type2);
}

void ViCorrelationGroup::setType1(ViAudio::Type type)
{
    mType1 = type;
}

void ViCorrelationGroup::setType2(ViAudio::Type type)
{
    mType2 = type;
}

void ViCorrelationGroup::clear()
{
    mType1 = ViAudio::Undefined;
    mType2 = ViAudio::Undefined;
    mCorrelations.clear();
}

int ViCorrelationGroup::count()
{
    return mCorrelations.size();
}

void ViCorrelationGroup::add(QString correlator, ViCorrelation correlation)
{
    if(mCorrelations.contains(correlator))
    {
        LOG("The correlator is already present in the group.", QtCriticalMsg);
    }
    else
    {
        mCorrelations[correlator] = correlation;
    }
}

void ViCorrelationGroup::add(ViCorrelator *correlator, ViCorrelation correlation)
{
    add(correlator->name(), correlation);
}

void ViCorrelationGroup::set(QString correlator, ViCorrelation correlation)
{
    mCorrelations[correlator] = correlation;
}

void ViCorrelationGroup::set(ViCorrelator *correlator, ViCorrelation correlation)
{
    set(correlator->name(), correlation);
}

ViCorrelation& ViCorrelationGroup::correlation(QString correlator)
{
    if(!mCorrelations.contains(correlator))
    {
        LOG("The correlator is not present in the group.", QtCriticalMsg);
    }
    return mCorrelations[correlator];
}

ViCorrelation& ViCorrelationGroup::correlation(ViCorrelator *correlator)
{
    return correlation(correlator->name());
}

ViCorrelations ViCorrelationGroup::correlations()
{
    ViCorrelations result;
    QString key;
    foreach(key, mCorrelations.keys())
    {
        result.append(mCorrelations[key]);
    }
    return result;
}

QStringList ViCorrelationGroup::correlators()
{
    return mCorrelations.keys();
}

QString ViCorrelationGroup::toString()
{
    QString result = "";
    QString key;
    foreach(key, mCorrelations.keys())
    {
        result += toString(key) + "\n";
    }
    result.replace(result.length() - 2, 1);
    return result;
}

QString ViCorrelationGroup::toString(QString correlator)
{
    if(mCorrelations.contains(correlator))
    {
        return correlator + "[" +  viTypeToString(type1()) + " - " + viTypeToString(type2()) + "] (" + mCorrelations[correlator].toString() + ")";
    }
    return "";
}

ViElement ViCorrelationGroup::exportData()
{
    ViElement root("correlationgroup");
    root.addChild("type1", viTypeToString(type1()));
    root.addChild("type2", viTypeToString(type2()));
    QString key;
    foreach(key, mCorrelations.keys())
    {
        ViElement correlator("correlator");
        correlator.addChild("name", key);
        correlator.addChild(mCorrelations[key].exportData());
        root.addChild(correlator);
    }
    return root;
}

bool ViCorrelationGroup::importData(ViElement element)
{
    if(element.name() != "correlationgroup")
    {
        return false;
    }
    clear();

    setType1(viStringToType(element.child("type1").toString()));
    setType2(viStringToType(element.child("type2").toString()));

    ViElementList children = element.children("correlator");
    for(int i = 0; i < children.size(); ++i)
    {
        ViCorrelation correlation;
        correlation.importData(children[i]);
        mCorrelations[children[i].child("name").toString()] = correlation;
    }
    return true;
}

bool ViCorrelationGroup::hasEqualTypes(const ViCorrelationGroup &other)
{
    return mType1 == other.mType1 && mType2 == other.mType2;
}

bool ViCorrelationGroup::operator ==(const ViCorrelationGroup &other)
{
    if(hasEqualTypes(other) && mCorrelations.size() == other.mCorrelations.size())
    {
        QString key;
        foreach(key, mCorrelations.keys())
        {
            if(!other.mCorrelations.contains(key) || mCorrelations[key] != other.mCorrelations[key])
            {
                return false;
            }
        }
    }
    return false;
}

bool ViCorrelationGroup::operator !=(const ViCorrelationGroup &other)
{
    return !(*this == other);
}
