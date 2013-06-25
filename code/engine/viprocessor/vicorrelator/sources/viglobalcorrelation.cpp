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

void ViGlobalCorrelation::add(ViCorrelations correlations)
{
    for(int i = 0; i < correlations.size(); ++i)
    {
        add(correlations[i]);
    }
}

void ViGlobalCorrelation::add(ViCorrelation correlation)
{
    mCorrelations.append(correlation);

    bool contains = false;
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        ViCorrelation &globalCorrelation = mCorrelations[i];
        if( globalCorrelation.type1() == correlation.type1() &&
            globalCorrelation.type2() == correlation.type2() &&
            globalCorrelation.correlator() == correlation.correlator()
        )
        {
            contains = true;
            break;
        }
    }

    if(!contains)
    {
        mCorrelations.append(ViCorrelation(correlation.correlator(), correlation.type1(), correlation.type2()));
    }

    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        ViCorrelation &globalCorrelation = mCorrelations[i];
        if( globalCorrelation.type1() == correlation.type1() &&
            globalCorrelation.type2() == correlation.type2() &&
            globalCorrelation.correlator() == correlation.correlator()
        )
        {
            globalCorrelation.addCorrelation(correlation.mean(), correlation.minimum(), correlation.maximum());
            break;
        }
    }
}

ViCorrelations ViGlobalCorrelation::correlations()
{
    return mCorrelations;
}

ViCorrelation ViGlobalCorrelation::correlation(QString correlator, ViAudio::Type type1, ViAudio::Type type2)
{
    for(int i = 0; i < mCorrelations.size(); ++i)
    {
        if( mCorrelations[i].type1() == type1 &&
            mCorrelations[i].type2() == type2 &&
            mCorrelations[i].correlator() == correlator
        )
        {
            return mCorrelations[i];
        }
    }
    return ViCorrelation();
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
        return false;
    }
    ViElementList elements = element.children("correlation");
    for(int i = 0; i < elements.size(); ++i)
    {
        ViCorrelation correlation;
        correlation.importData(elements[i]);
        add(correlation);
    }
    return true;
}
