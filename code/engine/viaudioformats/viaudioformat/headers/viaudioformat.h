#ifndef VIAUDIOFORMAT_H
#define VIAUDIOFORMAT_H

#include <QString>
#include <QList>
#include "visingleton.h"

class ViAudioFormat : public ViSingleton
{
	public:
		enum ViAudioFormatCompression
		{
			Lossy = 0,
			Lossless = 1,
			Variable = 3
		};
		static ViAudioFormat* instance();
		ViAudioFormat::ViAudioFormatCompression compression();
		QString abbreviation();
		QString name();
		QList<QString> extensions();
		QList<QString> starExtensions();
		bool operator ==(const ViAudioFormat &other) const;

	protected:
		ViAudioFormat();

	protected:
		static QSharedPointer<ViAudioFormat> mInstance;
		ViAudioFormat::ViAudioFormatCompression mCompression;
		QString mAbbreviation;
		QString mName;
		QList<QString> mExtensions;
};

#endif
