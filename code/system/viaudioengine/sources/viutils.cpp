#include <QAudioFormat>
#include "viutils.h"

qint64 audioDuration(const QAudioFormat &format, qint64 bytes)
{
	return (bytes * 1000000) / (format.frequency() * format.channels() * (format.sampleSize() / 8));
}

qint64 audioLength(const QAudioFormat &format, qint64 microSeconds)
{
	qint64 result = (format.frequency() * format.channels() * (format.sampleSize() / 8)) * microSeconds / 1000000;
	result -= result % (format.channelCount() * format.sampleSize());
	return result;
}

qreal nyquistFrequency(const QAudioFormat &format)
{
	return format.frequency() / 2;
}

QString formatToString(const QAudioFormat &format)
{
	QString result;

	if(QAudioFormat() != format)
	{
		if(format.codec() == "audio/pcm")
		{
			Q_ASSERT(format.sampleType() == QAudioFormat::SignedInt);

			const QString formatEndian = (format.byteOrder() == QAudioFormat::LittleEndian) ? QString("LE") : QString("BE");

			QString formatType;
			switch(format.sampleType())
			{
				case QAudioFormat::SignedInt:
					formatType = "signed";
					break;
				case QAudioFormat::UnSignedInt:
					formatType = "unsigned";
					break;
				case QAudioFormat::Float:
					formatType = "float";
					break;
				case QAudioFormat::Unknown:
					formatType = "unknown";
					break;
			}

			QString formatChannels = QString("%1 channels").arg(format.channels());
			switch(format.channels())
			{
				case 1:
					formatChannels = "mono";
					break;
				case 2:
					formatChannels = "stereo";
					break;
			}

			result = QString("%1 Hz %2 bit %3 %4 %5").arg(format.frequency()).arg(format.sampleSize()).arg(formatType).arg(formatEndian).arg(formatChannels);
		}
		else
		{
			result = format.codec();
		}
	}

	return result;
}

bool isPCM(const QAudioFormat &format)
{
	return (format.codec() == "audio/pcm");
}


bool isPCMS16LE(const QAudioFormat &format)
{
	return (isPCM(format) && format.sampleType() == QAudioFormat::SignedInt && format.sampleSize() == 16 && format.byteOrder() == QAudioFormat::LittleEndian);
}

const qint16  PCMS16MaxValue =  32767;
const quint16 PCMS16MaxAmplitude = 32768; // because minimum is -32768

qreal pcmToReal(qint16 pcm)
{
	return qreal(pcm) / PCMS16MaxAmplitude;
}

qint16 realToPcm(qreal real)
{
	return real * PCMS16MaxValue;
}
