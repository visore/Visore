#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include <viprocessor.h>

class ViCorrelator : public ViDualProcessor
{

    Q_OBJECT

    signals:

    void changed();

    public:

        enum Type
        {
            Mean,
            Minimum,
            Maximum
        };

	public:
		
        ViCorrelator();
        virtual ~ViCorrelator();

        virtual void initialize();
        virtual void finalize();

        qreal correlation(ViCorrelator::Type type = ViCorrelator::Mean);
        qreal meanCorrelation();
        qreal minimumCorrelation();
        qreal maximumCorrelation();

    protected:

        void addCorrelation(qreal mean);

        void setCorrelation(ViCorrelator::Type type, qreal correlation);
        void setMeanCorrelation(qreal correlation);
        void setMinimumCorrelation(qreal correlation);
        void setMaximumCorrelation(qreal correlation);

    private:

        qreal mMeanCorrelation;
        qreal mMinimumCorrelation;
        qreal mMaximumCorrelation;

        qreal mMeanTotal;
        int mCounter;

};

#endif
