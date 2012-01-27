#include "vilameencoder.h"

ViLameEncoder::ViLameEncoder()
	: ViAudioEncoder()
{
	mFormat = ViFormatManager::format("MP3");
	mName = "Lame";
	mPath = "lame";

	mParameters.append(new ViEncoderParameter(ViEncoderParameter::Integer, "Bitrate", "kbps", "-b", "256"));

	mParameters.append(new ViEncoderParameter(ViEncoderParameter::Bool, "Constant Bitrate", "", "--cbr", "true"));

	ViEncoderParameter *vbr = new ViEncoderParameter(ViEncoderParameter::Enumeration, "Variable Bitrate", "", "-V", "4");
	vbr->addPossibleValue("Ultimate Good", "0");
	vbr->addPossibleValue("Extreme Good", "1");
	vbr->addPossibleValue("Very Good", "2");
	vbr->addPossibleValue("Good", "3");
	vbr->addPossibleValue("Above Average", "4");
	vbr->addPossibleValue("Average", "5");
	vbr->addPossibleValue("Below Average", "6");
	vbr->addPossibleValue("Poor", "7");
	vbr->addPossibleValue("Very Poor", "8");
	vbr->addPossibleValue("Extreme Poor", "9");
	mParameters.append(vbr);

	mParameters.append(new ViEncoderParameter(ViEncoderParameter::Integer, "Maximum Variable Bitrate", "kbps", "-B", "320"));
	mParameters.append(new ViEncoderParameter(ViEncoderParameter::Integer, "Minimum Variable Bitrate", "kbps", "-b", "32"));

	ViEncoderParameter *quality = new ViEncoderParameter(ViEncoderParameter::Enumeration, "Quality", "", "-q", "2");
	quality->addPossibleValue("Ultimate Good", "0");
	quality->addPossibleValue("Extreme Good", "1");
	quality->addPossibleValue("Very Good", "2");
	quality->addPossibleValue("Good", "3");
	quality->addPossibleValue("Above Average", "4");
	quality->addPossibleValue("Average", "5");
	quality->addPossibleValue("Below Average", "6");
	quality->addPossibleValue("Poor", "7");
	quality->addPossibleValue("Very Poor", "8");
	quality->addPossibleValue("Extreme Poor", "9");
	mParameters.append(quality);

	ViEncoderParameter *mode = new ViEncoderParameter(ViEncoderParameter::Enumeration, "Mode", "", "-m", "j");
	mode->addPossibleValue("Mono", "m");
	mode->addPossibleValue("Dual Mono", "d");
	mode->addPossibleValue("Mid-Side (MS) Stereo", "f");
	mode->addPossibleValue("Left-Right (LR) Stereo", "s");
	mode->addPossibleValue("Joint MS-LR Stereo", "j");
	mParameters.append(mode);

	mParameters.append(new ViEncoderParameter(ViEncoderParameter::Integer, "Sample Frequency", "kHz", "-s", "44.1"));
}

ViAudioEncoder* ViLameEncoder::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViLameEncoder>(new ViLameEncoder).dynamicCast<ViAudioEncoder>();
	}
	return mInstance.data();
}

QString ViLameEncoder::string(QString filePath)
{
	QString result = mPath;
	if(parameter("Constant Bitrate")->value() == "true")
	{
		result += " " + parameter("Constant Bitrate")->string() + " " + parameter("Bitrate")->string();
	}
	else
	{
		result += " " + parameter("Variable Bitrate")->string() + " " + parameter("Maximum Variable Bitrate")->string() + " " + parameter("Minimum Variable Bitrate")->string();
	}
	result += " " + parameter("Mode")->string() + " " + parameter("Sample Frequency")->string() + " " + filePath;
	return result;
}

#ifdef __cplusplus
extern "C"
{
#endif

ViAudioEncoder* createEncoder()
{
   return ViLameEncoder::instance();
}

#ifdef __cplusplus
}
#endif
