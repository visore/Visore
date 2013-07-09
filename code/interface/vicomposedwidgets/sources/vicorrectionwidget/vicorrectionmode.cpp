#include <vicorrectionmode.h>

void ViCorrectionMode::setMode(ViCorrectionMode::Mode mode)
{
}

QString ViCorrectionMode::modeToString(ViCorrectionMode::Mode mode)
{
	if(mode == ViCorrectionMode::Custom)
	{
		return "Custom Correction";
	}
	else if(mode == ViCorrectionMode::Quick)
	{
		return "Quick Correction";
	}
	else if(mode == ViCorrectionMode::Medium)
	{
		return "Medium Correction";
	}
	else if(mode == ViCorrectionMode::Advanced)
	{
		return "Advanced Correction";
	}
	else if(mode == ViCorrectionMode::Extreme)
	{
		return "Extreme Correction";
	}
	return "";
}

ViCorrectionMode::Mode ViCorrectionMode::stringToMode(QString mode)
{
	mode = mode.toLower();
	if(mode.contains("quick"))
	{
		return ViCorrectionMode::Quick;
	}
	else if(mode.contains("medium"))
	{
		return ViCorrectionMode::Medium;
	}
	else if(mode.contains("advanced"))
	{
		return ViCorrectionMode::Advanced;
	}
	else if(mode.contains("extreme"))
	{
		return ViCorrectionMode::Extreme;
	}
	return ViCorrectionMode::Custom;
}

QStringList ViCorrectionMode::modes()
{
	QStringList result;
	result << modeToString(ViCorrectionMode::Custom);
	result << modeToString(ViCorrectionMode::Quick);
	result << modeToString(ViCorrectionMode::Medium);
	result << modeToString(ViCorrectionMode::Advanced);
	result << modeToString(ViCorrectionMode::Extreme);
	return result;
}

QString ViCorrectionMode::defaultMode()
{
	return modeToString(ViCorrectionMode::Quick);
}

