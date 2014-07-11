#ifndef VIAUDIODATA_H
#define VIAUDIODATA_H

#include <vibuffer.h>
#include <vipcmconverter.h>
#include <vifouriertransformer.h>

class ViAudioData
{

    public:

        ViAudioData();
        virtual ~ViAudioData();

        void clear();

		void setBuffer(ViBuffer *buffer);
        ViBuffer* buffer();
		qint64 bufferSize();
		qint64 bufferSamples();

        void setSampleCount(int samples);
		bool setWindowFunction(QString function);

        int sampleCount();
        int windowSize();
        int channelCount();

		qint64 position();

		void setScaleRange(int from, int to);

    protected:

        void setDefaults();
        void update();

		virtual void clearOther();
		virtual void defaultOther();
		virtual void updateOther();

    protected:

		QMutex mMutex;
        ViBuffer *mBuffer;
		ViBufferStreamPointer mStream;

        ViPcmConverter<qreal> mConverter;
		ViRawChunk mRawChunk;
        ViSampleChunk mSampleChunk;
        ViFrequencyChunk mFrequencyChunk;
		ViSampleChunk mChannleChunk;

        ViFourierTransformer mTransformer;

        int mWindowSize;
        int mSampleCount;
		int mSampleSize;
        int mChannelCount;

		int mScaleFrom;
		int mScaleTo;

};

class ViAudioReadData : public ViAudioData
{

    public:

		ViAudioReadData();
		ViAudioReadData(ViBuffer *buffer, const bool &reversed = false);

		void setReversed(const bool &reversed = true); // Read from end to start. Reversed reads will have the last channel first, and first channel last

        bool hasData();

		ViSampleChunk& read();

        ViSampleChunk& samples();
        ViSampleChunks& splitSamples();
        ViSampleChunk& splitSamples(int index);

		ViSampleChunk& scaledSamples();
		ViSampleChunks& scaledSplitSamples();
		ViSampleChunk& scaledSplitSamples(int index);
		ViSampleChunk& scaledSamples(const int &scaleFrom, const int &scaleTo);
		ViSampleChunks& scaledSplitSamples(const int &scaleFrom, const int &scaleTo);
		ViSampleChunk& scaledSplitSamples(int index, const int &scaleFrom, const int &scaleTo);

        ViFrequencyChunk& frequencies();
        ViFrequencyChunks& splitFrequencies();
        ViFrequencyChunk& splitFrequencies(int index);

    protected:

        void clearOther();
		void defaultOther();
        void updateOther();

    private:

		bool mReversed;

        ViSampleChunks mSplitSampleChunks;
        ViFrequencyChunks mSplitFrequencyChunks;

		ViSampleChunk mScaledSampleChunk;
		ViSampleChunks mScaledSplitSampleChunks;

        bool mNeedsSampleSplit;
        bool mNeedsFrequencySplit;
        bool mNeedsFrequency;
		bool mNeedsSampleScale;
		bool mNeedsSampleScaleSplit;

		ViFrequencyChunk mTemporaryChunk;

};

class ViAudioWriteData : public ViAudioData
{

	public:

		ViAudioWriteData();
		ViAudioWriteData(ViBuffer *buffer);

		//Enqueues samples. Once a chunk is available for each channel, they are written.
		void enqueueSplitSamples(ViSampleChunk &samples, const int &channel);
		void enqueueSplitScaledSamples(ViSampleChunk &samples, const int &channel);
		void enqueueSplitFrequencies(ViFrequencyChunk &frequencies, const int &channel);

		void write(ViSampleChunk &chunk);
		void writeScaled(ViSampleChunk &chunk);
		void writeFrequencies(ViFrequencyChunk &frequencies);
		void writeSplitSamples(ViSampleChunks &samples);
		void writeSplitScaledSamples(ViSampleChunks &samples);
		void writeSplitFrequencies(ViFrequencyChunks &frequencies);

    protected:

		qint64 writableSize();
		void dequeueSamples();

		void clearOther();
        void updateOther();

	private:

		QVector<ViSampleChunk> mChannelSamples;

};

#endif
