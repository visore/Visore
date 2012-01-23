/*
This is an adaption of the Qt Spectrum demo.
*/

#ifndef VISPECTRUM_H
#define VISPECTRUM_H

#include <QtGlobal>
#include "vifourierwrapper.h"
#include "viutils.h"

// Number of audio samples used to calculate the frequency spectrum
const int cSpectrumLengthSamples = PowerOfTwo<FFTLengthPowerOfTwo>::Result;

// Number of bands in the frequency spectrum
const int cSpectrumNumBands = 10;

// Lower bound of first band in the spectrum
const qreal cSpectrumLowFreq = 0.0; // Hz

// Upper band of last band in the spectrum
const qreal cSpectrumHighFreq = 1000.0; // Hz

// Waveform window size in microseconds
const qint64 cWaveformWindowDuration = 500 * 1000;

// Length of waveform tiles in bytes
// Ideally, these would match the QAudio*::bufferSize(), but that isn't
// available until some time after QAudio*::start() has been called, and we
// need this value in order to initialize the waveform display.
// We therefore just choose a sensible value.
const int cWaveformTileLength = 4096;

// Fudge factor used to calculate the spectrum bar heights
const qreal cSpectrumAnalyserMultiplier = 0.15;

// Disable message timeout
const int cNullMessageTimeout = -1;

enum ViWindowFunction
{
	NoWindow,
	HannWindow
};

const ViWindowFunction cDefaultWindowFunction = HannWindow;

#endif
