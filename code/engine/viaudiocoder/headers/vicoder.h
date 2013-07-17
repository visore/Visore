#ifndef VICODER_H
#define VICODER_H

#include <vicommon.h>
#include <string.h>

namespace ViCoder
{

	enum Error
	{
		NoError,
		UnknownError,

		InputFileError,
		OutputFileError,

		DecoderInitializationError,
		EncoderInitializationError,
		DecoderFinalizationError,
		EncoderFinalizationError,
		DecodingError,
		EncodingError,
		HeaderError,
		MetadataError,

		LibraryFileError,
		LibraryVersionError,

		UnsupportedCodecError,
		UnavailableCodecError,
		UnsupportedInputCodecError,
		UnavailableInputCodecError,
		UnsupportedOutputCodecError,
		UnavailableOutputCodecError,

		InputSampleSizeError,
		InputSampleTypeError,
		InputSampleRateError,
		InputBitrateError,
		InputChannelError,
		OutputSampleSizeError,
		OutputSampleTypeError,
		OutputSampleRateError,
		OutputBitrateError,
		OutputChannelError
		
	};

	QString errorString(ViCoder::Error error);

};

#endif
