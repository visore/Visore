#ifndef VICORRELATOR_H
#define VICORRELATOR_H

#include <viprocessor.h>
#include <vicorrelation.h>
#include <vilibrary.h>

class ViCorrelator : public ViDualProcessor
{

    Q_OBJECT

    signals:

        void changed();

	public:
		
        ViCorrelator();
        ViCorrelator(const ViCorrelator &other);
        virtual ~ViCorrelator();

        virtual void initialize();
        virtual void finalize();

        ViCorrelation correlation();

        ViElement exportData();
        bool importData(ViElement element);

        virtual ViCorrelator* clone() = 0;

    protected:

        void addCorrelation(qreal mean);
        void setCorrelation(ViCorrelation::Type type, qreal correlation);
        void setMeanCorrelation(qreal correlation);
        void setMinimumCorrelation(qreal correlation);
        void setMaximumCorrelation(qreal correlation);

    private:

        ViCorrelation mCorrelation;

};

#endif
