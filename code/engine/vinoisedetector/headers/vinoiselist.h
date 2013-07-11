#ifndef VINOISELIST_H
#define VINOISELIST_H

#include <vinoisewindow.h>
#include <vicommon.h>

class ViNoiseList
{

	public:

		ViNoiseList();
		ViNoiseList(const ViNoiseList &other);

		void clear();

		void setFormat(const ViAudioFormat &format);
		const ViAudioFormat& format() const;

		void setSeparateChannels(const bool &separate = true);
		void setCombinedChannels(const bool &combined = true);

		int count() const;
		int size() const;

		void add(const int &sampleIndex, const int &size, const int &channel);

		ViNoiseWindow at(const int &index) const;
		ViNoiseWindow operator [](const int &index) const;

	private:

		void updateChannelCount();

	private:

		ViAudioFormat mFormat;
		bool mSeperateChannels;
		ViIntList mIndexes;
		ViIntList mSizes;
		ViIntList mChannels;

};

#endif
