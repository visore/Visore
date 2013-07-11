#include <vinoiselist.h>

ViNoiseList::ViNoiseList()
{
	mSeperateChannels = false;
}

ViNoiseList::ViNoiseList(const ViNoiseList &other)
{
	mFormat = other.mFormat;
	mSeperateChannels = other.mSeperateChannels;
	mIndexes = other.mIndexes;
	mSizes = other.mSizes;
	mChannels = other.mChannels;
}

void ViNoiseList::clear()
{
	mIndexes.clear();
	mSizes.clear();
	mChannels.clear();
}

void ViNoiseList::setFormat(const ViAudioFormat &format)
{
	mFormat = format;
	updateChannelCount();
}

const ViAudioFormat& ViNoiseList::format() const
{
	return mFormat;
}

void ViNoiseList::setSeparateChannels(const bool &separate)
{
	mSeperateChannels = separate;
	updateChannelCount();
}

void ViNoiseList::setCombinedChannels(const bool &combined)
{
	mSeperateChannels = !combined;
	updateChannelCount();
}

int ViNoiseList::count() const
{
	return mIndexes.size();
}

int ViNoiseList::size() const
{
	return mIndexes.size();
}

void ViNoiseList::add(const int &sampleIndex, const int &size, const int &channel)
{
	mIndexes.append(sampleIndex);
	mSizes.append(size);
	mChannels.append(channel);
}

ViNoiseWindow ViNoiseList::at(const int &index) const
{
	return ViNoiseWindow(mIndexes[index], mSizes[index], mChannels[index], mFormat);
}

ViNoiseWindow ViNoiseList::operator [](const int &index) const
{
	return ViNoiseWindow(mIndexes[index], mSizes[index], mChannels[index], mFormat);
}

void ViNoiseList::updateChannelCount()
{
	// The audio position in ViNoiseWindow is calculated with the channel count.
	// If we use seperate channels, the relative position within the channel has to calculated
	/*if(mSeperateChannels)
	{
		mFormat.setChannelCount(1);
	}*/
}
