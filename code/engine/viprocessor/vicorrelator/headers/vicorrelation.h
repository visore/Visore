#ifndef VICORRELATION_H
#define VICORRELATION_H

#include <viserializer.h>
#include <vicommon.h>

class ViCorrelation : public ViSerializer
{

    public:

        enum Type
        {
            Mean,
            Minimum,
            Maximum
        };

    public:

        ViCorrelation();
        ViCorrelation(const ViCorrelation &other);

        void clear();
		bool isValid() const;

		qreal correlation(ViCorrelation::Type type) const;
		qreal mean() const;
		qreal minimum() const;
		qreal maximum() const;

        void addCorrelation(qreal value);
        void addCorrelation(qreal mean, qreal minimum, qreal maximum);
        void addMean(qreal mean);

        void setCorrelation(ViCorrelation::Type type, qreal correlation);
        void setMean(qreal correlation);
        void setMinimum(qreal correlation);
        void setMaximum(qreal correlation);

		QString toString() const;

        ViElement exportData();
        bool importData(ViElement element);

        bool operator >(const ViCorrelation &other);
        bool operator <(const ViCorrelation &other);
        bool operator ==(const ViCorrelation &other);
        bool operator !=(const ViCorrelation &other);

    private:

        qreal mMean;
        qreal mMinimum;
        qreal mMaximum;

        qreal mTotal;
        int mCounter;
};

typedef QList<ViCorrelation> ViCorrelationList;
typedef ViCorrelationList ViCorrelations;

#endif
