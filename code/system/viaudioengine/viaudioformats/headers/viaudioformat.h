#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <QString>
#include <QList>

class ViAudioFormat
{
	public:
		enum ViAudioFormatCompression
		{
			Lossy = 0,
			Lossless = 1,
			Variable = 3
		};
		ViAudioFormat::ViAudioFormatCompression compression();
		QString abbreviation();
		QString name();
		QList<QString> extensions();
		QList<QString> starExtensions();

	protected:
		ViAudioFormat::ViAudioFormatCompression mCompression;
		QString mAbbreviation;
		QString mName;
		QList<QString> mExtensions;
};

#endif
