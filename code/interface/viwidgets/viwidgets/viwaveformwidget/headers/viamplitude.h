#ifndef VIAMPLITUDE_H
#define VIAMPLITUDE_H

class ViAmplitude
{
	public:
		ViAmplitude(double maximum = 0, double minimum = 0, double maximumAverage = 0, double minimumAverage = 0);
		double maximum();
		double minimum();
		double maximumAverage();
		double minimumAverage();

	private:
		double mMaximum;
		double mMinimum;
		double mMaximumAverage;
		double mMinimumAverage;
};

#endif
