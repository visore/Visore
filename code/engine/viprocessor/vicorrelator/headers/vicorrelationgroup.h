#ifndef VICORRELATIONGROUP_H
#define VICORRELATIONGROUP_H

#include <vicorrelation.h>

class ViCorrelator;

class ViCorrelationGroup : public ViSerializer
{

    public:

        ViCorrelationGroup();
        ViCorrelationGroup(ViAudioTypePair pair);
        ViCorrelationGroup(ViAudio::Type type1, ViAudio::Type type2);
        ViCorrelationGroup(const ViCorrelationGroup &other);

        ViAudio::Type type1();
        ViAudio::Type type2();
        void setTypes(ViAudio::Type type1, ViAudio::Type type2);
        void setType1(ViAudio::Type type);
        void setType2(ViAudio::Type type);

        void clear();
        int count();

        void add(QString correlator, ViCorrelation correlation);
        void add(ViCorrelator *correlator, ViCorrelation correlation);

		void set(QString correlator, ViCorrelation correlation);
		void set(ViCorrelator *correlator, ViCorrelation correlation);

        ViCorrelation& correlation(QString correlator);
        ViCorrelation& correlation(ViCorrelator *correlator);

        ViCorrelations correlations();
        QStringList correlators();

        QString toString();
        QString toString(QString correlator);

        ViElement exportData();
        bool importData(ViElement element);

        bool hasEqualTypes(const ViCorrelationGroup &other);
        bool operator ==(const ViCorrelationGroup &other);
        bool operator !=(const ViCorrelationGroup &other);

    private:

        ViAudio::Type mType1;
        ViAudio::Type mType2;
        QHash<QString, ViCorrelation> mCorrelations;

};

typedef QList<ViCorrelationGroup> ViCorrelationGroupList;
typedef ViCorrelationGroupList ViCorrelationGroups;

#endif
