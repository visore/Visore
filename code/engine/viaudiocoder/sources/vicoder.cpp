#include <vicoder.h>

QString ViCoder::errorString(ViCoder::Error error)
{
	if(error == ViCoder::NoError)
	{
		return "No error.";
	}
	else if(error == ViCoder::InputFileError)
	{
		return "The input file could not be accessed.";
	}
	else if(error == ViCoder::OutputFileError)
	{
		return "The output file could not be accessed.";
	}

	else if(error == ViCoder::DecoderInitializationError)
	{
		return "The decoder could not be initialized.";
	}
	else if(error == ViCoder::DecoderFinalizationError)
	{
		return "The decoder could not be finalized and properly closed.";
	}
	else if(error == ViCoder::DecodingError)
	{
		return "The data could not be decoded.";
	}
	
	else if(error == ViCoder::EncoderInitializationError)
	{
		return "The encoder could not be initialized.";
	}
	else if(error == ViCoder::EncoderFinalizationError)
	{
		return "The encoder could not be finalized and properly closed.";
	}
	else if(error == ViCoder::EncodingError)
	{
		return "The data could not be encoded.";
	}

	else if(error == ViCoder::HeaderError)
	{
		return "The header is invalid.";
	}
	else if(error == ViCoder::MetadataError)
	{
		return "The metadata is invalid.";
	}

	else if(error == ViCoder::LibraryFileError)
	{
		return "The library file is invalid.";
	}
	else if(error == ViCoder::LibraryVersionError)
	{
		return "The library version is invalid.";
	}

	else if(error == ViCoder::UnsupportedCodecError)
	{
		return "The codec is not supported.";
	}
	else if(error == ViCoder::UnavailableCodecError)
	{
		return "The codec is supported, but not available.";
	}
	else if(error == ViCoder::UnsupportedInputCodecError)
	{
		return "The input codec is not supported.";
	}
	else if(error == ViCoder::UnavailableInputCodecError)
	{
		return "The input codec is supported, but not available.";
	}
	else if(error == ViCoder::UnsupportedOutputCodecError)
	{
		return "The output codec is not supported.";
	}
	else if(error == ViCoder::UnavailableOutputCodecError)
	{
		return "The output codec is supported, but not available.";
	}

	else if(error == ViCoder::InputSampleSizeError)
	{
		return "The input format (sample size) is invalid.";
	}
	else if(error == ViCoder::InputSampleTypeError)
	{
		return "The input format (sample type) is invalid.";
	}
	else if(error == ViCoder::InputSampleRateError)
	{
		return "The input format (sample rate) is invalid.";
	}
	else if(error == ViCoder::InputBitrateError)
	{
		return "The input format (bitrate) is invalid.";
	}
	else if(error == ViCoder::InputChannelError)
	{
		return "The input format (channels) is invalid.";
	}

	else if(error == ViCoder::OutputSampleSizeError)
	{
		return "The output format (sample size) is invalid.";
	}
	else if(error == ViCoder::OutputSampleTypeError)
	{
		return "The output format (sample type) is invalid.";
	}
	else if(error == ViCoder::OutputSampleRateError)
	{
		return "The output format (sample rate) is invalid.";
	}
	else if(error == ViCoder::OutputBitrateError)
	{
		return "The output format (bitrate) is invalid.";
	}
	else if(error == ViCoder::OutputChannelError)
	{
		return "The output format (channels) is invalid.";
	}

	return "Unknown error.";
}
