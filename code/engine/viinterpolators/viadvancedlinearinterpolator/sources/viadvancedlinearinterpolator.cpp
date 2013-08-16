#include <viadvancedlinearinterpolator.h>
#include<vilogger.h>
#include<iomanip>
#define DECAY 0.9
#define MAXIMUM_HISTORY 8//Don't take more than x left/right samples for the interpolation

ViAdvancedLinearInterpolator::ViAdvancedLinearInterpolator()
	: ViInterpolator()
{
	mUnits.resize(MAXIMUM_HISTORY + 1);
	for(int i = 0; i <= MAXIMUM_HISTORY; ++i) // <= because we start at 0, which is useless
	{
		mUnits[i] = 1.0 / (0.5 * i * i + 0.5 * i); // y = 0.5x^2 + 0.5x -> least square fit gave this function
	}
}

bool ViAdvancedLinearInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0)
	{
		if(rightSize == 0) return false;

/*		qreal value = rightValue(rightSamples, rightSize);
		qreal delta = rightDelta(rightSamples, rightSize);
		int i = outputSize - 1;
		while(i >= 0)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			--i;
		}*/
	}
	else if(rightSize == 0)
	{
		/*qreal value = leftValue(leftSamples, leftSize);
		qreal delta = leftDelta(leftSamples, leftSize);
		int i = 0;
		while(i < outputSize)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			++i;
		}*/
	}
	else
	{

		/*qreal *leftSamples2 = (qreal*)leftSamples;
		int leftSize2 =leftSize;
		qreal *rightSamples2 = (qreal*)rightSamples;
		int rightSize2=rightSize;
		qreal *outputSamples2 = (qreal*) outputSamples;
		int outputSize2 = outputSize;
		while(outputSize2>0)
		{
			if(outputSize == 1)
			{
				outputSamples2[0] = middleValue2(leftSamples2, leftSize2, rightSamples2, rightSize2, outputSize2, 0);
				outputSize2 = 0;
			}
			else
			{
				outputSamples2[0] = middleValue2(leftSamples2, leftSize2, rightSamples2, rightSize2, outputSize2, 0);
				outputSamples2[outputSize2-1] = middleValue2(leftSamples2, leftSize2, rightSamples2, rightSize2, outputSize2, outputSize2-1);
				outputSamples2 += 1;
				outputSize2 -= 2;
				leftSize2 += 1;
				rightSamples2 -= 1;
			}
		}*/






		/*qreal *leftSamples2 = (qreal*)leftSamples;
		int leftSize2 =leftSize;
		qreal *rightSamples2 = (qreal*)rightSamples;
		int rightSize2=rightSize;
		qreal *outputSamples2 = (qreal*) outputSamples;
		int outputSize2 = outputSize;
		while(outputSize2>0)
		{
			if(outputSize == 1)
			{
				outputSamples2[0] = middleValue2(leftSamples2, leftSize2, rightSamples2, rightSize2, outputSize2, 0);
				outputSize2 = 0;
			}
			else
			{
				outputSamples2[0] = leftValue(leftSamples2, leftSize2);
				outputSamples2[outputSize2-1] = rightValue(rightSamples2, rightSize2);
				outputSamples2 += 1;
				outputSize2 -= 2;
				leftSize2 += 1;
				rightSamples2 -= 1;
			}
		}*/




		/*int i = 0;
		while(i < outputSize)
		{
			//value += delta;
			outputSamples[i] = middleValue2(leftSamples, leftSize, rightSamples, rightSize, outputSize, i);
			++i;
		}*/



		int i = 0;

		qreal deltal = 0;
		qreal deltar = 0;
int counter = 0;

			qreal *leftSamples2 = (qreal*)leftSamples;
			int leftSize2 =leftSize;
			qreal *rightSamples2 = (qreal*)rightSamples;
			int rightSize2=rightSize;
			qreal *outputSamples2 = (qreal*) outputSamples;
			int outputSize2 = outputSize;
			while(outputSize2>0)
			{


				if(outputSize2 == 2)
				{
					/*qreal a = (outputSize2 - i)/qreal(outputSize2+1);
					qreal b = 1-a;
					qreal delta1 = (deltal*a) + (deltar*b);
					qreal delta2 = (deltal*b) + (deltar*a);

				   // outputSamples2[outputSize2-1] = rightSamples2[0] + delta;

					outputSamples2[0] = ((leftSamples2[leftSize2-1] + delta1) + (rightSamples2[0] + delta2))/2;

					outputSize2 = 0;*/
					outputSamples2[0] = (leftSamples2[leftSize2-1] * 0.8) + (rightSamples2[0] * 0.2);
					outputSamples2[1] = (leftSamples2[leftSize2-1] * 0.2) + (rightSamples2[0] * 0.8);

							outputSize2 = 0;
				}
				else
				{
					++counter;

					//int rat = qCeil((counter/qreal(outputSize))*MAXIMUM_HISTORY);
					//int rat = qMax(1, qMin(MAXIMUM_HISTORY-counter-1,MAXIMUM_HISTORY));
					int rat = MAXIMUM_HISTORY;
					//cout<<"* "<<rat<<endl;

					deltal = leftDelta(leftSamples2, leftSize2, rat);
					deltar = rightDelta(rightSamples2, rightSize2,rat);

					qreal valuel = leftValue(leftSamples2, leftSize2,rat);
					qreal valuer = rightValue(rightSamples2, rightSize2,rat);

					cout<<fixed<<setprecision(4);






					qreal aq = (outputSize2 - 0)/qreal(outputSize2+1);
					qreal bq = 1-aq;


					qreal a = (outputSize2-counter)/qreal(outputSize2+1);


					//qreal a = (-counter)/qreal(outputSize2+1);
					qreal b = 1-a;
					qreal delta1, delta2;
					//++counter;

					//delta1 = (deltal*a) + (deltar*b);
					//delta2 = (deltal*b) + (deltar*a);

					delta1 = (valuer - valuel) / qreal(outputSize2+1);
					delta2= (valuel - valuer) / qreal(outputSize2+1);

					//cout<<"*: "<<delta1<<" "<<valuel<<endl;

					//delta1 = (rightSamples2[0] - leftSamples2[leftSize2-1]) / qreal(outputSize2+1);
					//delta2= (leftSamples2[leftSize2-1] - rightSamples2[0]) / qreal(outputSize2+1);

					//delta1 = (rightSamples[0] - leftSamples[leftSize-1]) / qreal(outputSize+1);
					//delta2= (leftSamples[leftSize-1] - rightSamples[0]) / qreal(outputSize+1);

					//delta1 = ((valuer - valuel) / qreal(outputSize2+1));
					//delta2=delta1;

					//	cout<<delta1<<" "<<delta2<<" "<<valuel<<" "<<valuer<<endl;

					qreal adaptedLeft = valuel + delta1;
					qreal adaptedRight = valuer + delta2;

					qreal p = (rightSamples[0] - leftSamples[leftSize-1]) / qreal(outputSize+1);

				//	adaptedLeft = (adaptedLeft*b) +(a*(leftSamples2[leftSize2-1] + (leftSamples2[leftSize2-1] - leftSamples2[leftSize2-2])));
					//	adaptedRight = (adaptedRight*b) +(a*(rightSamples2[0] + (rightSamples2[0] - rightSamples2[1])));

					adaptedLeft = (adaptedLeft*b) +(a*(leftSamples2[leftSize-1]+p));
					adaptedRight = (adaptedRight*b) +(a*(rightSamples2[0]+p));

					/*if(counter==1)
					{
						adaptedLeft = leftSamples[leftSize-1];
						adaptedRight = rightSamples[0];
					}*/

					outputSamples2[0] = adaptedLeft;
					//outputSamples2[0] = leftSamples2[leftSize2-1] + delta;
					//cout<<"*1:  "<<a<<"\t"<<b<<"\t"<<deltal<<"\t"<<deltar<<"\t"<<delta<<"\t"<<outputSamples2[0]<<endl;
					//outputSamples2[0] = (valuel * aq) + (valuer * bq);

					outputSamples2[outputSize2-1] = adaptedRight;
					//outputSamples2[outputSize2-1] = rightSamples[0] + delta;
					//cout<<"*2:  "<<a<<"\t"<<b<<"\t"<<deltal<<"\t"<<deltar<<"\t"<<delta<<"\t"<<outputSamples2[outputSize2-1]<<endl;
					 //outputSamples2[outputSize2-1] = (valuel * bq) + (valuer * aq);






					outputSamples2 += 1;
					outputSize2 -= 2;
					leftSize2 += 1;
					rightSamples2 -= 1;
				}
			}



		/*while(i < outputSize)
		{
			qreal a = (outputSize - i)/qreal(outputSize+1);
			qreal b = 1-a;
			qreal delta = (deltal*a) + (deltar*b);
			//cout<<a<<" "<<b<<" "<<deltal<<" "<<deltar<<" "<<delta<<endl;


			value += delta;
			outputSamples[i] = value;
			++i;
		}*/
	}
	return true;
}

qreal ViAdvancedLinearInterpolator::leftValue(const qreal *leftSamples, const int &leftSize, const int &history)
{
	int size = qMin(leftSize, history);
	qreal value = 0, unit = mUnits[size];
	for(int i = 0; i < size; ++i)
	{
		value += leftSamples[leftSize - i - 1] * (unit * (size - i));
	}
	return value;

	/*int size = qMin(leftSize, MAXIMUM_HISTORY);
	qreal value = leftSamples[leftSize - 1], unit = mUnits[size];
	for(int i = 0; i < size-1; ++i)
	{
		value += (leftSamples[leftSize - i - 1] - leftSamples[leftSize - i - 2]) * (unit * (size - i));
	}
	return value;*/
}

qreal ViAdvancedLinearInterpolator::rightValue(const qreal *rightSamples, const int &rightSize, const int &history)
{
	int size = qMin(rightSize, history);
	qreal value = 0, unit = mUnits[size];
	for(int i = 0; i < size; ++i)
	{
		value += rightSamples[i] * (unit * (size - i));
	}
	return value;
	/*int size = qMin(rightSize, MAXIMUM_HISTORY);
		qreal value = rightSamples[0], unit = mUnits[size];
		for(int i = 0; i < size-1; ++i)
		{
			value += (rightSamples[i]-rightSamples[i+1]) * (unit * (size - i));
		}
		return value;*/
}

qreal ViAdvancedLinearInterpolator::middleValue2(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize, int i)
{
	//qreal unit = mUnits[outputSize];
	qreal a = (outputSize - i)/qreal(outputSize+1);
	qreal b = 1-a;

	/*qreal a;
	qreal b;
	if(i < outputSize / 2)
	{
		a = 1;
		b = 0;
	}
	else
	{
		a = 0.0;
		b = 1;
	}*/

//	return (leftValue(leftSamples, leftSize) * a) + (rightValue(rightSamples, rightSize) * b);
}

qreal ViAdvancedLinearInterpolator::leftDelta(const qreal *leftSamples, const int &leftSize, const int &history)
{
	if(leftSize < 2) return 0;
	int size = qMin(leftSize, history) - 1;
	qreal value = 0, unit = mUnits[size];
	for(int i = 0; i < size; ++i)
	{
		value += (leftSamples[leftSize - i - 1] - leftSamples[leftSize - i - 2]) * (unit * (size - i));
	}
	return value;
}

qreal ViAdvancedLinearInterpolator::rightDelta(const qreal *rightSamples, const int &rightSize, const int &history)
{
	if(rightSize < 2) return 0;
	int size = qMin(rightSize, history) - 1;
	qreal value = 0, unit = mUnits[size];
	for(int i = 0; i < size; ++i)
	{
		value += (rightSamples[i] - rightSamples[i + 1]) * (unit * (size - i));
	}
	return value;
}

qreal ViAdvancedLinearInterpolator::middleDelta(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize)
{
//	return ViLinearInterpolator::middleDelta(leftSamples, leftSize, rightSamples, rightSize, outputSize);
	//return (leftDelta(leftSamples, leftSize)) / (outputSize + 1);
}

ViAdvancedLinearInterpolator* ViAdvancedLinearInterpolator::clone()
{
	return new ViAdvancedLinearInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViAdvancedLinearInterpolator* create()
{
	return new ViAdvancedLinearInterpolator();
}

#ifdef __cplusplus
}
#endif
