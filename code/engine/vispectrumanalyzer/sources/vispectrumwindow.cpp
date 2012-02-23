#include "vispectrumwindow.h"

ViSpectrumWindow::ViSpectrumWindow(qint64 numberOfSamples, ViWindowFunction *function)
{
	for(int i = 0; i < numberOfSamples; ++i)
	{
		append(function->calculate(i, numberOfSamples));
	}
}
