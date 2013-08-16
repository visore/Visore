#ifndef VIADVANCEDLINEARINTERPOLATOR_H
#define VIADVANCEDLINEARINTERPOLATOR_H

#include <viinterpolator.h>

class ViAdvancedLinearInterpolator : public ViInterpolator
{

	public:

		ViAdvancedLinearInterpolator();

		ViAdvancedLinearInterpolator* clone();

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		qreal leftValue(const qreal *leftSamples, const int &leftSize, const int &history);
		qreal rightValue(const qreal *rightSamples, const int &rightSize, const int &history);
		qreal middleValue2(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, int i);

		qreal leftDelta(const qreal *leftSamples, const int &leftSize, const int &history);
		qreal rightDelta(const qreal *rightSamples, const int &rightSize, const int &history);
		qreal middleDelta(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize);

	private:

		QVector<qreal> mUnits;

};

#endif

