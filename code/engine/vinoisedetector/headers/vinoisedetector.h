#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <viserializer.h>
#include <vilibrary.h>
#include <viprocessor.h>

class ViNoiseDetector : public ViSerializer, public ViLibrary
{

	public:
		
        ViNoiseDetector();
		ViNoiseDetector(ViProcessor::ChannelMode mode);
        ViNoiseDetector(const ViNoiseDetector &other);
        virtual ~ViNoiseDetector();

		void setMode(ViProcessor::ChannelMode mode);
		ViProcessor::ChannelMode mode();

		void setChannel(int channel);
		int channel();

		void setData(ViAudioReadData &data);
		bool hasData();

		bool isNoisy(int channel);
		bool isNoisy(ViAudioReadData &data);
		bool isNoisy(ViAudioReadData &data, int channel);
        virtual bool isNoisy() = 0;

        ViIntList noisyWindows();

        void clear();

        ViElement exportData();
        bool importData(ViElement element);

        virtual ViNoiseDetector* clone() = 0;

	protected:

		ViSampleChunk& samples();
		ViFrequencyChunk& frequencies();

    private:

		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
        int mCounter;
        ViIntList mNoise;

};

#endif
