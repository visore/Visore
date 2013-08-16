#ifndef VIPREDICTORS_H
#define VIPREDICTORS_H

#include <vilibrary.h>
#include <vichunk.h>

class ViPredictor : public ViLibrary
{

	public:
		
		ViPredictor();
		ViPredictor(const int &minimumSamples);
		ViPredictor(const ViPredictor &other);
		virtual ~ViPredictor();

		qreal predict(const ViSampleChunk &samples);
		qreal predict(const ViSampleChunk &samples, const int &index);
		QList<qreal> predict(const int &length, const ViSampleChunk &samples);
		QList<qreal> predict(const int &length, const ViSampleChunk &samples, const int &index);

		virtual ViPredictor* clone() = 0;

	protected:

		virtual qreal predictValue(const qreal *samples, const int &size) = 0;
		virtual QList<qreal> predictValues(const qreal *samples, const int &size, const int &length) = 0;

		void setMinimumSamples(const int &samples);

		bool enoughData(const ViSampleChunk &samples, const int &startIndex) const;

    private:

		int mMinimumSamples;

};

#endif
