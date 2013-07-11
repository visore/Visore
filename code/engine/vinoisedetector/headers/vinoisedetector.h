#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <viserializer.h>
#include <vilibrary.h>
#include <viprocessor.h>
#include <vinoiselist.h>

class ViNoiseDetector : public ViSerializer, public ViLibrary
{

	public:
		
        ViNoiseDetector();
		ViNoiseDetector(ViProcessor::ChannelMode mode);
        ViNoiseDetector(const ViNoiseDetector &other);
        virtual ~ViNoiseDetector();

		void setFormat(const ViAudioFormat &format);

		void setMode(ViProcessor::ChannelMode mode);
		ViProcessor::ChannelMode mode();

		void setChannel(int channel);
		int channel();

		void setData(ViAudioReadData &data);
		bool hasData();

		bool isNoisy(int channel);
		bool isNoisy(ViAudioReadData &data);
		bool isNoisy(ViAudioReadData &data, int channel);
		bool isNoisy();

		ViNoiseList noisyWindows();

        void clear();

        ViElement exportData();
        bool importData(ViElement element);

        virtual ViNoiseDetector* clone() = 0;

	protected:

		virtual bool calculateNoise() = 0;

		ViSampleChunk& samples();
		ViFrequencyChunk& frequencies();

    private:

		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
        int mCounter;
		ViNoiseList mNoise;

};

#endif
