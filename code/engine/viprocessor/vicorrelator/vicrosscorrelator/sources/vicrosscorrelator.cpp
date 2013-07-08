#include <vicrosscorrelator.h>

ViCrossCorrelator::ViCrossCorrelator()
	: ViCorrelator()
{
	mRealData = NULL;
	mFirstFourierData = NULL;
	mSecondFourierData = NULL;
	mMultiplyData = NULL;
	mAutocorrelationData = NULL;
}

ViCrossCorrelator::ViCrossCorrelator(const ViCrossCorrelator &other)
    : ViCorrelator(other)
{
    mTransformer = other.mTransformer;
    mRealData = NULL;
	mFirstFourierData = NULL;
	mSecondFourierData = NULL;
    mMultiplyData = NULL;
    mAutocorrelationData = NULL;
}

ViCrossCorrelator::~ViCrossCorrelator()
{
	deallocateData();
}

void ViCrossCorrelator::initialize()
{
	ViCorrelator::initialize();
	cout<<"+++++++++++"<<sampleCount()<<endl;
	mTransformer.setSize(sampleCount());
	deallocateData();
	allocateData();
}

void ViCrossCorrelator::finalize()
{
	deallocateData();
    ViCorrelator::finalize();
}

void ViCrossCorrelator::execute(int channel)
{
	ViSampleChunk &theSamples1 = currentSamples();
	ViSampleChunk &theSamples2 = currentSamples2();

    qreal firstNorm, secondNorm, multipliedNorm;
    int count = sampleCount();

	memcpy(mRealData, theSamples1.data(), theSamples1.size() * sizeof(qreal));
	firstNorm = norm(mRealData, theSamples1.size());
	multipliedNorm = firstNorm * firstNorm;
	mTransformer.pad(mRealData, theSamples1.size());
    mTransformer.forwardTransform(mRealData, mFirstFourierData);

	//Auto-correlation
    memcpy(mRealData, mFirstFourierData, count * sizeof(qreal));
	mTransformer.conjugate(mFirstFourierData);
	mTransformer.multiply(mFirstFourierData, mRealData, mMultiplyData);	
	mTransformer.inverseTransform(mMultiplyData, mAutocorrelationData);
	mTransformer.rescale(mAutocorrelationData);
	applyNorm(mAutocorrelationData, count, multipliedNorm);

	//Cross-correlation
	memcpy(mRealData, theSamples2.data(), theSamples2.size() * sizeof(qreal));
	secondNorm = norm(mRealData, theSamples2.size());
	multipliedNorm = firstNorm * secondNorm;
	mTransformer.pad(mRealData, theSamples2.size());
	mTransformer.forwardTransform(mRealData, mSecondFourierData);
	mTransformer.multiply(mFirstFourierData, mSecondFourierData, mMultiplyData);
	mTransformer.inverseTransform(mMultiplyData, mRealData);
	mTransformer.rescale(mRealData);
	applyNorm(mRealData, count, multipliedNorm);

    for(int i = 0; i < count; ++i)
	{
       addCorrelation(1 - (qAbs(mRealData[i] - mAutocorrelationData[i]) / 2));
	}
	/*cout<<mRealData[0]<<" ";
	cout <<mAutocorrelationData[0] << "  ";
	cout<<(1 - (qAbs(mRealData[0] - mAutocorrelationData[0]) / 2))<<endl;*/
}

ViCrossCorrelator* ViCrossCorrelator::clone()
{
    return new ViCrossCorrelator(*this);
}

void ViCrossCorrelator::allocateData()
{
	mRealData = new qreal[sampleCount()];
	mFirstFourierData = new qreal[sampleCount()];
	mSecondFourierData = new qreal[sampleCount()];
	mMultiplyData = new qreal[sampleCount()];
	mAutocorrelationData = new qreal[sampleCount()];
}

void ViCrossCorrelator::deallocateData()
{
	if(mRealData != NULL)
	{
		delete [] mRealData;
		mRealData = NULL;
	}
	if(mFirstFourierData != NULL)
	{
		delete [] mFirstFourierData;
		mFirstFourierData = NULL;
	}
	if(mSecondFourierData != NULL)
	{
		delete [] mSecondFourierData;
		mSecondFourierData = NULL;
	}
	if(mMultiplyData != NULL)
	{
		delete [] mMultiplyData;
		mMultiplyData = NULL;
	}
	if(mAutocorrelationData != NULL)
	{
		delete [] mAutocorrelationData;
		mAutocorrelationData = NULL;
	}
}

qreal ViCrossCorrelator::norm(qreal array[], qint32 size)
{
	qreal norm = 0;
	for(int i = 0; i < size; ++i)
	{
		norm += (array[i] * array[i]);
	}
	return qSqrt(norm);
}

void ViCrossCorrelator::applyNorm(qreal array[], qint32 size, qreal norm)
{
	if(norm != 0)
	{
		for(int i = 0; i < size; ++i)
		{
			array[i] /= norm;
		}
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

ViCrossCorrelator* create()
{
    return new ViCrossCorrelator();
}

#ifdef __cplusplus
}
#endif
