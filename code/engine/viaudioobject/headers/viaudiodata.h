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

        void setSampleCount(int samples);
        int sampleCount();
        int windowSize();
        int channelCount();

    protected:

        void update();
        virtual void clearOther() = 0;
        virtual void updateOther() = 0;

    protected:

        ViBuffer *mBuffer;
        ViBufferStreamPointer mStream;

        ViPcmConverter<qreal> mConverter;
        ViRawChunk mRawChunk;
        ViSampleChunk mSampleChunk;
        ViFrequencyChunk mFrequencyChunk;

        ViFourierTransformer mTransformer;

        int mWindowSize;
        int mSampleCount;
        int mChannelCount;

};

class ViAudioReadData : public ViAudioData
{

    public:

        ViSampleChunk& read(); // Read next window

        ViSampleChunk& samples();
        ViSampleChunks& splitSamples();
        ViSampleChunk& splitSamples(int index);

        ViFrequencyChunk& frequencies();
        ViFrequencyChunks& splitFrequencies();
        ViFrequencyChunk& splitFrequencies(int index);

    protected:

        void clearOther();
        void updateOther();

    private:

        ViSampleChunks mSplitSampleChunks;
        ViFrequencyChunks mSplitFrequencyChunks;

        bool mNeedsSampleSplit;
        bool mNeedsFrequencySplit;
        bool mNeedsFrequency;

        ViFrequencyChunk mTemporaryChunk;

};

class ViAudioWriteData : public ViAudioData
{

    public:

      void write(ViSampleChunk &chunk); // Write next window
      void writeFrequencies(ViFrequencyChunk &frequencies);
      void writeSplitSamples(ViSampleChunks &samples);
      void writeSplitFrequencies(ViFrequencyChunks &frequencies);

    protected:

        void clearOther();
        void updateOther();

    private:

};

#endif
