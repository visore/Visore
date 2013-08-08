#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <vilibrary.h>
#include <viprocessor.h>
#include <vinoise.h>

class ViNoiseDetector : public ViLibrary
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
		bool isNoisy();

		ViNoise& noise();

        void clear();

        virtual ViNoiseDetector* clone() = 0;

	protected:

		virtual void calculateNoise(ViNoise &noise, const ViSampleChunk &samples) = 0;

    private:

		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
		ViNoise mNoise;

};

#endif
