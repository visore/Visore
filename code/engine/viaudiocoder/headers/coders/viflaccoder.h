#ifndef VIFLACCODER_H
#define VIFLACCODER_H

#include <viabstractcoder.h>
#include <visamplesizeconverter.h>
#include <all.h>
#include <QThread>
#include <QWaitCondition>

class ViFlacCoder : public ViAbstractCoder, public QThread
{

	public:

		ViFlacCoder();
		~ViFlacCoder();

		ViAudioCodec* detectCodec(const QByteArray &data);
		QByteArray& header();
		int headerSize();

		bool initializeEncode();
		bool finalizeEncode();
		void encode(const void *input, int samples);

		bool initializeDecode();
		bool finalizeDecode();
		void decode(const void *input, int size);
		void run();

	protected:

		void pause();
		void resume();
		void wait();
		int dataSize();
		bool isPaused();

		void encode8Convert(const void *input, int samples);
		void encode16Convert(const void *input, int samples);
		void encode32Convert(const void *input, int samples);
		void encode8Normal(const void *input, int samples);
		void encode16Normal(const void *input, int samples);
		void encode32Normal(const void *input, int samples);

		static FLAC__StreamEncoderWriteStatus flacWriteEncode(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client);
		static FLAC__StreamEncoderWriteStatus flacWriteEncodeHeader(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client);
		static FLAC__StreamEncoderWriteStatus flacWriteEncodeData(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client);
		static void flacMetadataEncode(const FLAC__StreamEncoder *encoder, const FLAC__StreamMetadata *metadata, void *client);
		static FLAC__StreamEncoderWriteStatus flacWriteHeaderEncode(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client);

		static void flacErrorDecode(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client);
		static void flacMetadataDecode(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client);
		static FLAC__StreamDecoderReadStatus flacReadDecode(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client);
		static FLAC__StreamDecoderWriteStatus flacWriteDecode(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client);
		static FLAC__StreamDecoderWriteStatus flacWriteDecode8(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client);
		static FLAC__StreamDecoderWriteStatus flacWriteDecode16(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client);
		static FLAC__StreamDecoderWriteStatus flacWriteDecode32(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client);

		ViCoder::Error initializeLibrary();

	private:

		FLAC__StreamEncoder* (*m_FLAC__stream_encoder_new)();
		void (*m_FLAC__stream_encoder_delete)(FLAC__StreamEncoder*);
		FLAC__StreamEncoderInitStatus (*m_FLAC__stream_encoder_init_stream)(FLAC__StreamEncoder*, FLAC__StreamEncoderWriteCallback, FLAC__StreamEncoderSeekCallback, FLAC__StreamEncoderTellCallback, FLAC__StreamEncoderMetadataCallback, void*);
		FLAC__bool (*m_FLAC__stream_encoder_finish)(FLAC__StreamEncoder*);

		FLAC__bool (*m_FLAC__stream_encoder_set_verify)(FLAC__StreamEncoder*, FLAC__bool);
		FLAC__bool (*m_FLAC__stream_encoder_set_compression_level)(FLAC__StreamEncoder*, unsigned);
		FLAC__bool (*m_FLAC__stream_encoder_set_channels)(FLAC__StreamEncoder*, unsigned);
		FLAC__bool (*m_FLAC__stream_encoder_set_bits_per_sample)(FLAC__StreamEncoder*, unsigned);
		FLAC__bool (*m_FLAC__stream_encoder_set_sample_rate)(FLAC__StreamEncoder*, unsigned);
		FLAC__bool (*m_FLAC__stream_encoder_set_total_samples_estimate)(FLAC__StreamEncoder*, FLAC__uint64);

		FLAC__bool (*m_FLAC__stream_encoder_set_metadata)(FLAC__StreamEncoder*, FLAC__StreamMetadata**, unsigned);
		FLAC__StreamMetadata* (*m_FLAC__metadata_object_new)(FLAC__MetadataType);
		void (*m_FLAC__metadata_object_delete)(FLAC__StreamMetadata*);
		FLAC__bool (*m_FLAC__metadata_object_vorbiscomment_append_comment)(FLAC__StreamMetadata*, FLAC__StreamMetadata_VorbisComment_Entry, FLAC__bool);
		FLAC__bool (*m_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair)(FLAC__StreamMetadata_VorbisComment_Entry*, const char*, const char*);
		FLAC__bool (*m_FLAC__metadata_object_picture_set_mime_type)(FLAC__StreamMetadata*, char*, FLAC__bool);
		FLAC__bool (*m_FLAC__metadata_object_picture_set_description)(FLAC__StreamMetadata*, FLAC__byte*, FLAC__bool);
		FLAC__bool (*m_FLAC__metadata_object_picture_set_data)(FLAC__StreamMetadata*, FLAC__byte*, FLAC__uint32, FLAC__bool);
		FLAC__bool (*m_FLAC__metadata_object_picture_is_legal)(const FLAC__StreamMetadata *object, const char **violation);

		FLAC_API FLAC__StreamMetadata *FLAC__metadata_object_new(FLAC__MetadataType type);

		FLAC__bool (*m_FLAC__stream_encoder_process_interleaved)(FLAC__StreamEncoder*, const FLAC__int32[], unsigned);

		
		FLAC__StreamDecoder* (*m_FLAC__stream_decoder_new)();
		void (*m_FLAC__stream_decoder_delete)(FLAC__StreamDecoder*);
		FLAC__bool (*m_FLAC__stream_decoder_finish)(FLAC__StreamDecoder*);
		FLAC__StreamDecoderInitStatus (*m_FLAC__stream_decoder_init_stream)(FLAC__StreamDecoder*, FLAC__StreamDecoderReadCallback, FLAC__StreamDecoderSeekCallback, FLAC__StreamDecoderTellCallback, FLAC__StreamDecoderLengthCallback, FLAC__StreamDecoderEofCallback, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void*);

		FLAC__bool (*m_FLAC__stream_decoder_process_until_end_of_stream)(FLAC__StreamDecoder*);

	private:

		void (ViFlacCoder::*encodePointer)(const void *input, int samples);
		FLAC__StreamEncoderWriteStatus (*flacWriteEncodePointer)(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t numberOfBytes, unsigned numberOfSamples, unsigned currentFrame, void *client);

		ViSampleSizeConverter mConverter;
		FLAC__StreamEncoder *mEncoder;
		FLAC__StreamDecoder *mDecoder;

		int mHeaderPosition;

		QMutex mMutex;
		QByteArray mData;

		QMutex mPauser;
		QWaitCondition mPauseCondition;
		bool mIsPaused;
		bool mAtEnd;

		QMutex mWaiter;
		QWaitCondition mWaitCondition;

};

#endif
