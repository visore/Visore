#include <vicorrelator.h>

ViCorrelator::ViCorrelator()
    : ViDualProcessor()
{
}

ViCorrelator::ViCorrelator(const ViCorrelator &other)
{
    mCorrelation = other.mCorrelation;
}

ViCorrelator::~ViCorrelator()
{
}

void ViCorrelator::initialize()
{
	setChannelMode(ViProcessor::Combined);
	setProcessMode(ViProcessor::All);
    mCorrelation.clear();
}

void ViCorrelator::finalize()
{
     LOG(mCorrelation.toString());
}

ViCorrelation ViCorrelator::correlation()
{
    return mCorrelation;
}

void ViCorrelator::addCorrelation(qreal mean)
{
    mCorrelation.addCorrelation(mean);
    emit changed();
}

void ViCorrelator::setCorrelation(ViCorrelation::Type type, qreal correlation)
{
    mCorrelation.setCorrelation(type, correlation);
}

void ViCorrelator::setMeanCorrelation(qreal correlation)
{
    mCorrelation.setMean(correlation);
}

void ViCorrelator::setMinimumCorrelation(qreal correlation)
{
    mCorrelation.setMinimum(correlation);
}

void ViCorrelator::setMaximumCorrelation(qreal correlation)
{
    mCorrelation.setMaximum(correlation);
}

ViElement ViCorrelator::exportData()
{
    return mCorrelation.exportData();
}

bool ViCorrelator::importData(ViElement element)
{
    return mCorrelation.importData(element);
}
