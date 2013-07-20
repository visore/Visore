#include <vispectrumanalyzer.h>

ViSpectrumAnalyzer::ViSpectrumAnalyzer()
	: ViProcessor()
{
	mSpectrum = NULL;
}

ViSpectrumAnalyzer::~ViSpectrumAnalyzer()
{
	clear();
}

void ViSpectrumAnalyzer::clear()
{
	if(mSpectrum != NULL)
	{
		delete mSpectrum;
		mSpectrum = NULL;
	}
	ViProcessor::clear();
}

void ViSpectrumAnalyzer::initialize()
{
	setChannelMode(ViProcessor::Combined);
	setProcessMode(ViProcessor::All);
	if(mSpectrum != NULL)
	{
		delete mSpectrum;
	}
	mSpectrum = new ViRealSpectrum(sampleCount(), format(), false);
}

void ViSpectrumAnalyzer::execute(const int &channel)
{
	mSpectrum->add(currentFrequencies());
}

void ViSpectrumAnalyzer::finalize()
{
	mSpectrum->update();
}

ViRealSpectrum* ViSpectrumAnalyzer::spectrum()
{
	return mSpectrum;
}

ViRealSpectrum* ViSpectrumAnalyzer::takeSpectrum()
{
	ViRealSpectrum *spectrum = mSpectrum;
	mSpectrum = NULL;
	return spectrum;
}
