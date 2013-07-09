#ifndef VICROSSCORRELATOR_H
#define VICROSSCORRELATOR_H

#include <vicorrelator.h>
#include <vifouriertransformer.h>

class ViCrossCorrelator : public ViCorrelator
{

	Q_OBJECT

	public:
		
        ViCrossCorrelator();
        ViCrossCorrelator(const ViCrossCorrelator &other);
		~ViCrossCorrelator();

		void initialize();
		void finalize();
		void execute(const int &channel);

        ViCrossCorrelator* clone();

	protected:

		void allocateData();
		void deallocateData();
		qreal norm(qreal array[], qint32 size);
		void applyNorm(qreal array[], qint32 size, qreal norm);

	private:

		ViFourierTransformer mTransformer;

		qreal *mRealData;
		qreal *mFirstFourierData;
		qreal *mSecondFourierData;
		qreal *mMultiplyData;
		qreal *mAutocorrelationData;

};

#endif
