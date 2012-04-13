#ifndef VICODECS_H
#define VICODECS_H

#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include "visingleton.h"

class ViCodecs : public ViSingleton
{
	public:
		enum Status
		{
			Success = 0,
			UnsupportedCodecError = 1,
			CodecOpenError = 2,
			OutOfMemoryError = 3,
			DecodingError = 4
		};
		static ViCodecs* instance();
		static int codecId(QString file);
		static ViCodecs::Status encode(QByteArray *inData, QByteArray *outData);
		static ViCodecs::Status decode(QByteArray *inData, QByteArray *outData, int ffmpegCodec);

	protected:
		ViCodecs();
		
	protected:
		static QSharedPointer<ViCodecs> mInstance;
};

#endif
