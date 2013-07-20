#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include <viprocessor.h>
#include <vispectrum.h>
#include <vilibrary.h>

class ViSpectrumAnalyzer : public ViProcessor, public ViLibrary
{

    Q_OBJECT

	public:
		
		ViSpectrumAnalyzer();
		virtual ~ViSpectrumAnalyzer();

		void clear();

		void initialize();
		void execute(const int &channel);
		void finalize();

		ViRealSpectrum* spectrum(); // Keeps ownership of the spectrum
		ViRealSpectrum* takeSpectrum(); // Ownership of spectrum moved to caller

    private:

		ViRealSpectrum *mSpectrum;

};

#endif
