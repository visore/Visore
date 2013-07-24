#include <viglobalcorrelation.h>

ViGlobalCorrelation::ViGlobalCorrelation()
{
}

ViGlobalCorrelation::ViGlobalCorrelation(const ViGlobalCorrelation &other)
{
    mCorrelations = other.mCorrelations;
}

void ViGlobalCorrelation::clear()
{
    mCorrelations.clear();
}

void ViGlobalCorrelation::add(ViCorrelationGroups correlations)
{
    for(int i = 0; i < correlations.size(); ++i)
    {
        add(correlations[i]);
    }
}

void ViGlobalCorrelation::add(ViCorrelationGroup correlation)
{
    mCorrelations.append(correlation);

    bool contains = false;
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        if(mCorrelations[i].hasEqualTypes(correlation))
        {
            contains = true;
            break;
        }
    }

    if(!contains)
    {
        mCorrelations.append(ViCorrelationGroup(correlation.type1(), correlation.type2()));
    }

    QString key;
    QStringList keys;
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        ViCorrelationGroup &globalCorrelation = mCorrelations[i];
        if(globalCorrelation.hasEqualTypes(correlation))
        {
            keys = correlation.correlators();
            foreach(key, keys)
			{
                ViCorrelation &currentCorrelation = correlation.correlation(key);
                globalCorrelation.correlation(key).addCorrelation(currentCorrelation.mean(), currentCorrelation.minimum(), currentCorrelation.maximum());
            }
            break;
        }
    }
}

ViCorrelationGroups ViGlobalCorrelation::correlations()
{
    return mCorrelations;
}

ViCorrelationGroup ViGlobalCorrelation::correlation(ViAudio::Type type1, ViAudio::Type type2)
{
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        if(mCorrelations[i].type1() == type1 && mCorrelations[i].type2() == type2)
        {
            return mCorrelations[i];
        }
    }
    return ViCorrelationGroup();
}

ViElement ViGlobalCorrelation::exportData()
{
    ViElement root("globalcorrelations");
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        root.addChild(mCorrelations[i].exportData());
    }
    return root;
}

bool ViGlobalCorrelation::importData(ViElement element)
{
    if(element.name() != "globalcorrelations")
    {
        if(element.hasChild("globalcorrelations")) element = element.child("globalcorrelations");
        else return false;
    }
    ViElementList children = element.children();
    for(int i = 0; i < children.size(); ++i)
    {
        ViCorrelationGroup group;
        group.importData(children[i]);
        mCorrelations.append(group);
    }
    return true;
}
