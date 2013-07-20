#ifndef VIAUDIODATA_H
#define VIAUDIODATA_H

#include <vibuffer.h>
#include <vipcmconverter.h>
#include <vifouriertransformer.h>

class ViAudioData
{

    public:

        ViAudioData();
        ViAudioData(ViBuffer *buffer);
        virtual ~ViAudioData();

        void clear();

        void setBuffer(ViBuffer *buffer);
        ViBuffer* buffer();
        int bufferSize();

        void setSampleCount(int samples);
		bool setWindowFunction(QString function);

        int sampleCount();
        int windowSize();
        int channelCount();

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
        int mChannelCount;

		int mScaleFrom;
		int mScaleTo;

};

class ViAudioReadData : public ViAudioData
{

    public:

        bool hasData();

		ViSampleChunk& read();

        ViSampleChunk& samples();
        ViSampleChunks& splitSamples();
        ViSampleChunk& splitSamples(int index);

		ViSampleChunk& scaledSamples();
		ViSampleChunks& scaledSplitSamples();
		ViSampleChunk& scaledSplitSamples(int index);

        ViFrequencyChunk& frequencies();
        ViFrequencyChunks& splitFrequencies();
        ViFrequencyChunk& splitFrequencies(int index);

    protected:

        void clearOther();
		void defaultOther();
        void updateOther();

    private:

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

		void dequeueSamples();

		void clearOther();
        void updateOther();

	private:

		QVector<QQueue<ViSampleChunk>> mChannelSamples;

};

#endif
