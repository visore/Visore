#ifndef VIWAVEFORMCHUNK_H
#define VIWAVEFORMCHUNK_H

#include <QtGlobal>
#include "viaudiometadata.h"

class ViWaveFormChunk
{
	public:
		ViWaveFormChunk(qreal *data = NULL, qint64 size = 0, ViAudioMetaData *metaData = NULL);
		~ViWaveFormChunk();
		void clear();
		qreal* data();
		qint64 size();
		qint8 bytesPerSample();
		ViAudioMetaData* metaData();
	
	private:
		qreal *mData;
		qint64 mSize;
		qint8 mBytesPerSample;
		ViAudioMetaData *mMetaData;
};

#endif
