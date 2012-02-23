#ifndef VIAUDIOENCODER_H
#define VIAUDIOENCODER_H

#include "viencoderparameter.h"
#include "viformatmanager.h"
#include "visingleton.h"
#include <QSharedPointer>

class ViAudioEncoder : public ViSingleton
{
	public:
		static ViAudioEncoder* instance();
		QString name();
		QString path();
		QList<ViEncoderParameter*> parameters();
		ViAudioFormat* format();
		ViEncoderParameter* parameter(QString name);
		void setPath(QString path);
		virtual QString string(QString filePath) = 0;
		~ViAudioEncoder(); //Must be public due to QSharedPointer
		
	protected:
		ViAudioEncoder();

	protected:
		static QSharedPointer<ViAudioEncoder> mInstance;
		QList<ViEncoderParameter*> mParameters;
		QString mName;
		QString mPath;
		ViAudioFormat *mFormat;
};

#endif
