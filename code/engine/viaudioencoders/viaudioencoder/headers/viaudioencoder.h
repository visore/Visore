#ifndef VIAUDIOENCODER_H
#define VIAUDIOENCODER_H

#include "viencoderparameter.h"
#include "viaudioformat.h"
#include <QSharedPointer>

class ViAudioEncoder
{
	public:
		static ViAudioEncoder* instance();
		QString name();
		QString path();
		QList<ViEncoderParameter*> parameters();
		//ViAudioFormat* format();
		ViEncoderParameter* parameter(QString name);
		void setPath(QString path);
		~ViAudioEncoder(); //Must be public due to QSharedPointer
		
	protected:
		ViAudioEncoder();

	protected:
		static QSharedPointer<ViAudioEncoder> mInstance;
		QList<ViEncoderParameter*> mParameters;
		QString mName;
		QString mPath;
};

#endif
