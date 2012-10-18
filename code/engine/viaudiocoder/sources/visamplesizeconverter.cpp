#include <visamplesizeconverter.h>

#define RANGE_8 128u
#define RANGE_16 32768u
#define RANGE_32 2147483648u
#define RANGE_FLOAT 1u
#define RANGE_REAL 1u

#define RANGE_8f 128.0
#define RANGE_16f 32768.0
#define RANGE_32f 2147483648.0
#define RANGE_FLOATf 1.0
#define RANGE_REALf 1.0

#define RATIO_16_8 257.0
#define RATIO_32_8 16843009.0

#define RATIO_8_16 0.00389105058365759
#define RATIO_32_16 65537.0

#define RATIO_8_32 5.93718141455603e-08
#define RATIO_16_32 1.5258556235409e-05

ViSampleSizeConverter::ViSampleSizeConverter()
{
	convert = NULL;
}

ViSampleSizeConverter::ViSampleSizeConverter(int inputSize, ViAudioFormat::SampleType inputType, int outputSize, ViAudioFormat::SampleType outputType)
{
	ViSampleSizeConverter();
	initialize(inputSize, inputType, outputSize, outputType);
}

bool ViSampleSizeConverter::initialize(int inputSize, ViAudioFormat::SampleType inputType, int outputSize, ViAudioFormat::SampleType outputType)
{
	convert = NULL;
	if(inputType == ViAudioFormat::SignedInt)
	{
		if(outputType == ViAudioFormat::SignedInt)
		{
			if(inputSize == 8)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert8sTo8s;
				if(outputSize == 16) convert = &ViSampleSizeConverter::convert8sTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert8sTo32s;
			}
			else if(inputSize == 16)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert16sTo8s;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert16sTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert16sTo32s;
			}
			else if(inputSize == 32)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert32sTo8s;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert32sTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert32sTo32s;
			}
		}
		else if(outputType == ViAudioFormat::UnSignedInt)
		{
			if(inputSize == 8)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert8sTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert8sTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert8sTo32u;
			}
			else if(inputSize == 16)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert16sTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert16sTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert16sTo32u;
			}
			else if(inputSize == 32)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert32sTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert32sTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert32sTo32u;
			}
		}
		else if(outputType == ViAudioFormat::Float)
		{
			if(inputSize == 8) convert = &ViSampleSizeConverter::convert8sToFloat;
			else if(inputSize == 16) convert = &ViSampleSizeConverter::convert16sToFloat;
			else if(inputSize == 32) convert = &ViSampleSizeConverter::convert32sToFloat;
		}
		else if(outputType == ViAudioFormat::Real)
		{
			if(inputSize == 8) convert = &ViSampleSizeConverter::convert8sToReal;
			else if(inputSize == 16) convert = &ViSampleSizeConverter::convert16sToReal;
			else if(inputSize == 32) convert = &ViSampleSizeConverter::convert32sToReal;
		}
	}
	else if(inputType == ViAudioFormat::UnSignedInt)
	{
		if(outputType == ViAudioFormat::SignedInt)
		{
			if(inputSize == 8)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert8uTo8s;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert8uTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert8uTo32s;
			}
			else if(inputSize == 16)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert16uTo8s;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert16uTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert16uTo32s;
			}
			else if(inputSize == 32)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert32uTo8s;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert32uTo16s;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert32uTo32s;
			}
		}
		else if(outputType == ViAudioFormat::UnSignedInt)
		{
			if(inputSize == 8)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert8uTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert8uTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert8uTo32u;
			}
			else if(inputSize == 16)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert16uTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert16uTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert16uTo32u;
			}
			else if(inputSize == 32)
			{
				if(outputSize == 8) convert = &ViSampleSizeConverter::convert32uTo8u;
				else if(outputSize == 16) convert = &ViSampleSizeConverter::convert32uTo16u;
				else if(outputSize == 32) convert = &ViSampleSizeConverter::convert32uTo32u;
			}
		}
		else if(outputType == ViAudioFormat::Float)
		{
			if(inputSize == 8) convert = &ViSampleSizeConverter::convert8uToFloat;
			else if(inputSize == 16) convert = &ViSampleSizeConverter::convert16uToFloat;
			else if(inputSize == 32) convert = &ViSampleSizeConverter::convert32uToFloat;
		}
		else if(outputType == ViAudioFormat::Real)
		{
			if(inputSize == 8) convert = &ViSampleSizeConverter::convert8uToReal;
			else if(inputSize == 16) convert = &ViSampleSizeConverter::convert16uToReal;
			else if(inputSize == 32) convert = &ViSampleSizeConverter::convert32uToReal;
		}
	}
	else if(inputType == ViAudioFormat::Float)
	{
		if(outputType == ViAudioFormat::SignedInt)
		{
			if(outputSize == 8) convert = &ViSampleSizeConverter::convertFloatTo8s;
			else if(outputSize == 16) convert = &ViSampleSizeConverter::convertFloatTo16s;
			else if(outputSize == 32) convert = &ViSampleSizeConverter::convertFloatTo32s;
		}
		else if(outputType == ViAudioFormat::UnSignedInt)
		{
			if(outputSize == 8) convert = &ViSampleSizeConverter::convertFloatTo8u;
			else if(outputSize == 16) convert = &ViSampleSizeConverter::convertFloatTo16u;
			else if(outputSize == 32) convert = &ViSampleSizeConverter::convertFloatTo32u;
		}
		else if(outputType == ViAudioFormat::Float)
		{
			convert = &ViSampleSizeConverter::convertFloatToFloat;
		}
		else if(outputType == ViAudioFormat::Real)
		{
			convert = &ViSampleSizeConverter::convertFloatToReal;
		}
	}
	else if(inputType == ViAudioFormat::Real)
	{
		if(outputType == ViAudioFormat::SignedInt)
		{
			if(outputSize == 8) convert = &ViSampleSizeConverter::convertRealTo8s;
			else if(outputSize == 16) convert = &ViSampleSizeConverter::convertRealTo16s;
			else if(outputSize == 32) convert = &ViSampleSizeConverter::convertRealTo32s;
		}
		else if(outputType == ViAudioFormat::UnSignedInt)
		{
			if(outputSize == 8) convert = &ViSampleSizeConverter::convertRealTo8u;
			else if(outputSize == 16) convert = &ViSampleSizeConverter::convertRealTo16u;
			else if(outputSize == 32) convert = &ViSampleSizeConverter::convertRealTo32u;
		}
		else if(outputType == ViAudioFormat::Float)
		{
			convert = &ViSampleSizeConverter::convertRealToFloat;
		}
		else if(outputType == ViAudioFormat::Real)
		{
			convert = &ViSampleSizeConverter::convertRealToReal;
		}
	}

	return convert != NULL;
}


/**********************************************************************
Value conversion - unsigned 8
**********************************************************************/

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte8u value)
{
	return value;
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte16u value)
{
	return ROUND(value / RATIO_16_8);
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte32u value)
{
	return ROUND(value / RATIO_32_8);
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte8s value)
{
	return value + RANGE_8;
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte16s value)
{
	return ROUND((value + RANGE_16) / RATIO_16_8);
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qbyte32s value)
{
	return ROUND((value + RANGE_32) / RATIO_32_8);
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qfloat value)
{
	return ROUND((value + RANGE_FLOAT) * RANGE_8);
}

qbyte8u ViSampleSizeConverter::convertTo8u(const qreal value)
{
	return ROUND((value + RANGE_REAL) * RANGE_8);
}

/**********************************************************************
Value conversion - unsigned 16
**********************************************************************/

qbyte16u ViSampleSizeConverter::convertTo16u(const qbyte8u value)
{
	return ROUND(value / RATIO_8_16);
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qbyte16u value)
{
	return value;
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qbyte32u value)
{
	return ROUND(value / RATIO_32_16);
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qbyte8s value)
{
	return ROUND((value + RANGE_8) / RATIO_8_16);
}

qbyte16u ViSampleSizeConverter::convertTo16u(qbyte16s value)
{
	return value + RANGE_16;
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qbyte32s value)
{
	return ROUND((value + RANGE_32) / RATIO_32_16);
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qfloat value)
{
	return ROUND((value + RANGE_FLOAT) * RANGE_16);
}

qbyte16u ViSampleSizeConverter::convertTo16u(const qreal value)
{
	return ROUND((value + RANGE_REAL) * RANGE_16);
}

/**********************************************************************
Value conversion - unsigned 32
**********************************************************************/

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte8u value)
{
	return ROUND(value / RATIO_8_32);
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte16u value)
{
	return ROUND(value / RATIO_16_32);
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte32u value)
{
	return value;
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte8s value)
{
	return ROUND((value + RANGE_8) / RATIO_8_32);
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte16s value)
{
	return ROUND((value + RANGE_16) / RATIO_16_32);
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qbyte32s value)
{
	return value + RANGE_32;
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qfloat value)
{
	return ROUND((value + RANGE_FLOAT) * RANGE_32);
}

qbyte32u ViSampleSizeConverter::convertTo32u(const qreal value)
{
	return ROUND((value + RANGE_REAL) * RANGE_32);
}

/**********************************************************************
Value conversion - signed 8
**********************************************************************/

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte8u value)
{
	return value - RANGE_8;
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte16u value)
{
	return ROUND((value - RANGE_16) / RATIO_16_8);
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte32u value)
{
	return ROUND((value - RANGE_32) / RATIO_32_8);
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte8s value)
{
	return value;
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte16s value)
{
	return ROUND(value / RATIO_16_8);
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qbyte32s value)
{
	return ROUND(value / RATIO_32_8);
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qfloat value)
{
	return ROUND(value * RANGE_8);
}

qbyte8s ViSampleSizeConverter::convertTo8s(const qreal value)
{
	return ROUND(value * RANGE_8);
}

/**********************************************************************
Value conversion - signed 16
**********************************************************************/

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte8u value)
{
	return ROUND((value - RANGE_8) / RATIO_8_16);
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte16u value)
{
	return value - RANGE_16;
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte32u value)
{
	return ROUND((value - RANGE_32) / RATIO_32_16);
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte8s value)
{
	return ROUND(value / RATIO_8_16);
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte16s value)
{
	return value;
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qbyte32s value)
{
	return ROUND(value / RATIO_32_16);
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qfloat value)
{
	return ROUND(value * RANGE_16);
}

qbyte16s ViSampleSizeConverter::convertTo16s(const qreal value)
{
	return ROUND(value * RANGE_16);
}

/**********************************************************************
Value conversion - signed 32
**********************************************************************/

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte8u value)
{
	return ROUND((value - RANGE_8) / RATIO_8_32);
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte16u value)
{
	return ROUND((value - RANGE_16) / RATIO_16_32);
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte32u value)
{
	return value - RANGE_32;
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte8s value)
{
	return ROUND(value / RATIO_8_32);
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte16s value)
{
	return ROUND(value / RATIO_16_32);
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qbyte32s value)
{
	return value;
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qfloat value)
{
	return ROUND(value * RANGE_32);
}

qbyte32s ViSampleSizeConverter::convertTo32s(const qreal value)
{
	return ROUND(value * RANGE_32);
}

/**********************************************************************
Value conversion - float
**********************************************************************/

qfloat ViSampleSizeConverter::convertToFloat(const qbyte8u value)
{
	return (value - RANGE_8) / RANGE_8f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qbyte16u value)
{
	return (value - RANGE_16) / RANGE_16f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qbyte32u value)
{
	return (value - RANGE_32) / RANGE_32f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qbyte8s value)
{
	return value / RANGE_8f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qbyte16s value)
{
	return value / RANGE_16f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qbyte32s value)
{
	return value / RANGE_32f;
}

qfloat ViSampleSizeConverter::convertToFloat(const qfloat value)
{
	return value;
}

qfloat ViSampleSizeConverter::convertToFloat(const qreal value)
{
	return value;
}

/**********************************************************************
Value conversion - real
**********************************************************************/

qreal ViSampleSizeConverter::convertToReal(const qbyte8u value)
{
	return (value - RANGE_8) / RANGE_8f;
}

qreal ViSampleSizeConverter::convertToReal(const qbyte16u value)
{
	return (value - RANGE_16) / RANGE_16f;
}

qreal ViSampleSizeConverter::convertToReal(const qbyte32u value)
{
	return (value - RANGE_32) / RANGE_32f;
}

qreal ViSampleSizeConverter::convertToReal(const qbyte8s value)
{
	return value / RANGE_8f;
}

qreal ViSampleSizeConverter::convertToReal(const qbyte16s value)
{
	return value / RANGE_16f;
}

qreal ViSampleSizeConverter::convertToReal(const qbyte32s value)
{
	return value / RANGE_32f;
}

qreal ViSampleSizeConverter::convertToReal(const qfloat value)
{
	return value;
}

qreal ViSampleSizeConverter::convertToReal(const qreal value)
{
	return value;
}

/**********************************************************************
Array conversion - unsigned 8
**********************************************************************/

void ViSampleSizeConverter::convert8uTo8u(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte8u));
}

void ViSampleSizeConverter::convert8uTo16u(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uTo32u(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uTo8s(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uTo16s(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uTo32s(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uToFloat(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8uToReal(const void *input, void *output, const int samples)
{
	qbyte8u *inputData = (qbyte8u*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - unsigned 16
**********************************************************************/

void ViSampleSizeConverter::convert16uTo8u(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uTo16u(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte16u));
}

void ViSampleSizeConverter::convert16uTo32u(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uTo8s(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uTo16s(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uTo32s(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uToFloat(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16uToReal(const void *input, void *output, const int samples)
{
	qbyte16u *inputData = (qbyte16u*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - unsigned 32
**********************************************************************/

void ViSampleSizeConverter::convert32uTo8u(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uTo16u(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uTo32u(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte32u));
}

void ViSampleSizeConverter::convert32uTo8s(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uTo16s(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uTo32s(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uToFloat(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32uToReal(const void *input, void *output, const int samples)
{
	qbyte32u *inputData = (qbyte32u*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - signed 8
**********************************************************************/

void ViSampleSizeConverter::convert8sTo8u(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sTo16u(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sTo32u(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sTo8s(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte8s));
}

void ViSampleSizeConverter::convert8sTo16s(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sTo32s(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sToFloat(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert8sToReal(const void *input, void *output, const int samples)
{
	qbyte8s *inputData = (qbyte8s*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - signed 16
**********************************************************************/

void ViSampleSizeConverter::convert16sTo8u(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sTo16u(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sTo32u(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sTo8s(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sTo16s(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte16s));
}

void ViSampleSizeConverter::convert16sTo32s(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sToFloat(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	 qfloat *outputData = ( qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert16sToReal(const void *input, void *output, const int samples)
{
	qbyte16s *inputData = (qbyte16s*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - signed 32
**********************************************************************/

void ViSampleSizeConverter::convert32sTo8u(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sTo16u(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sTo32u(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sTo8s(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sTo16s(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sTo32s(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qbyte32s));
}

void ViSampleSizeConverter::convert32sToFloat(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convert32sToReal(const void *input, void *output, const int samples)
{
	qbyte32s *inputData = (qbyte32s*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - float
**********************************************************************/

void ViSampleSizeConverter::convertFloatTo8u(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatTo16u(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatTo32u(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatTo8s(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatTo16s(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatTo32s(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertFloatToFloat(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qfloat));
}

void ViSampleSizeConverter::convertFloatToReal(const void *input, void *output, const int samples)
{
	qfloat *inputData = (qfloat*) input;
	qreal *outputData = (qreal*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToReal(inputData[i]);
	}
}

/**********************************************************************
Array conversion - real
**********************************************************************/

void ViSampleSizeConverter::convertRealTo8u(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte8u *outputData = (qbyte8u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealTo16u(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte16u *outputData = (qbyte16u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealTo32u(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte32u *outputData = (qbyte32u*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32u(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealTo8s(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte8s *outputData = (qbyte8s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo8s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealTo16s(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte16s *outputData = (qbyte16s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo16s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealTo32s(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qbyte32s *outputData = (qbyte32s*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertTo32s(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealToFloat(const void *input, void *output, const int samples)
{
	qreal *inputData = (qreal*) input;
	qfloat *outputData = (qfloat*) output;
	for(int i = 0; i < samples; ++i)
	{
		outputData[i] = ViSampleSizeConverter::convertToFloat(inputData[i]);
	}
}

void ViSampleSizeConverter::convertRealToReal(const void *input, void *output, const int samples)
{
	memcpy(output, input, samples * sizeof(qreal));
}

