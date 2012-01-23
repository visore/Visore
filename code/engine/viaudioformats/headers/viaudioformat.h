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

		ViAudioFormat::ViAudioFormatCompression compression()
		{
			return mCompression;
		}

		QString abbreviation()
		{
			return mAbbreviation;
		}

		QString name()
		{
			return mName;
		}

		QList<QString> extensions()
		{
			return mExtensions;
		}

		QList<QString> starExtensions()
		{
			QList<QString> list;
			for(int i = 0; i < mExtensions.size(); ++i)
			{
				list.append("*." + mExtensions[i]);
			}
			return list;
		}

	protected:
		ViAudioFormat::ViAudioFormatCompression mCompression;
		QString mAbbreviation;
		QString mName;
		QList<QString> mExtensions;
};

#endif
