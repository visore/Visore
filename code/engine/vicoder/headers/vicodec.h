#ifndef VICODEC_H
#define VICODEC_H

#include <QString>
#include <QList>

class ViCodec
{
	public:

		enum Compression
		{
			UndefinedCompression = 0,
			LossyCompression = 1,
			LosslessCompression = 2,
			VariableCompression = 3
		};
		enum Type
		{
			UndefinedType = 0,
			InputType = 1,
			OutputType = 5,
			InputOutputType = 6
		};

	public:

		ViCodec();
		ViCodec(ViCodec::Type type, ViCodec::Compression compression, QString abbreviation, QString name, QList<QString> extensions);
		ViCodec::Type type();
		ViCodec::Compression compression();
		QString abbreviation();
		QString name();
		QList<QString> extensions();
		QList<QString> starExtensions();
		bool operator ==(const ViCodec &other) const;

	protected:

		ViCodec::Type mType;
		ViCodec::Compression mCompression;
		QString mAbbreviation;
		QString mName;
		QList<QString> mExtensions;
};

#endif
