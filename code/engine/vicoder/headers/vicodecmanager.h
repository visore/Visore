#ifndef VICODECMANAGER_H
#define VICODECMANAGER_H

#include <QSharedPointer>
#include "vicodec.h"

class ViCodecManager
{
	public:
		static QList<ViCodec> all();
		static QList<ViCodec> selected(ViCodec::Type type);
		static QList<ViCodec> selected(ViCodec::Compression compression);
		static ViCodec selected(QString abbreviation);
		
	protected:
		ViCodecManager();
		static ViCodecManager* instance();
		void populate();

	private:
		static QSharedPointer<ViCodecManager> mInstance;
		QList<ViCodec> mCodecs;
};

#endif
