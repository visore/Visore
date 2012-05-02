#ifndef VICODECMANAGER_H
#define VICODECMANAGER_H

#include <QSharedPointer>
#include <QAudioFormat>
#include "vicodec.h"

class ViCodecManager
{
	public:
		static QList<ViCodec> all();
		static QList<ViCodec> selected(ViCodec::Type type);
		static QList<ViCodec> selected(ViCodec::Compression compression);
		static ViCodec selected(QString abbreviation);

		static QList<QAudioFormat::Endian> byteOrders();
		static QList<QAudioFormat::SampleType> sampleTypes();
		static QList<qint32> sampleRates();
		static QList<qint8> sampleSizes();
		static QList<qint8> channels();
		
	protected:
		ViCodecManager();
		static ViCodecManager* instance();
		void populateCodecs();
		void populateFormats();

	private:
		static QSharedPointer<ViCodecManager> mInstance;
		QList<ViCodec> mCodecs;

		QList<QAudioFormat::Endian> mByteOrders;
		QList<QAudioFormat::SampleType> mSampleTypes;
		QList<qint32> mSampleRates;
		QList<qint8> mSampleSizes;
		QList<qint8> mChannels;
};

#endif
