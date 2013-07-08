#ifndef VIGLOBALCORRELATION_H
#define VIGLOBALCORRELATION_H

#include <vicorrelationgroup.h>

class ViGlobalCorrelation : public ViSerializer
{

    public:

        ViGlobalCorrelation();
        ViGlobalCorrelation(const ViGlobalCorrelation &other);

        void clear();

        void add(ViCorrelationGroups correlations);
        void add(ViCorrelationGroup correlation);

        ViCorrelationGroups correlations();
        ViCorrelationGroup correlation(ViAudio::Type type1, ViAudio::Type type2);

        ViElement exportData();
        bool importData(ViElement element);

    private:

        ViCorrelationGroups mCorrelations;

};

#endif
