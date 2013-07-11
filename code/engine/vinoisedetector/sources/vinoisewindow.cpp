#include <vinoisewindow.h>

ViNoiseWindow::ViNoiseWindow()
{
	mStartIndex = 0;
	mSize = 0;
	mChannel = 0;
}

ViNoiseWindow::ViNoiseWindow(const int &startIndex, const int &size, const int &channel, const ViAudioFormat &format)
{
	mStartIndex = startIndex;
	mSize = size;
	mChannel = channel;
	mFormat = format;
}

ViNoiseWindow::ViNoiseWindow(const ViNoiseWindow &other)
{
	mStartIndex = other.mStartIndex;
	mSize = other.mSize;
	mChannel = other.mChannel;
	mFormat = other.mFormat;
}

void ViNoiseWindow::setStartIndex(const int &startIndex)
{
	mStartIndex = startIndex;
}

int ViNoiseWindow::startIndex() const
{
	return mStartIndex;
}

void ViNoiseWindow::setSize(const int &size)
{
	mSize = size;
}

int ViNoiseWindow::size() const
{
	return mSize;
}

void ViNoiseWindow::setChannel(const int &channel)
{
	mChannel = channel;
}

int ViNoiseWindow::channel() const
{
	return mChannel;
}

void ViNoiseWindow::setFormat(const ViAudioFormat &format)
{
	mFormat = format;
}

ViAudioFormat ViNoiseWindow::format() const
{
	return mFormat;
}

qreal ViNoiseWindow::from(const ViAudioPosition::Unit &unit) const
{
	return ViAudioPosition::convertPosition(mStartIndex, ViAudioPosition::Samples, unit, mFormat);
}

qreal ViNoiseWindow::to(const ViAudioPosition::Unit &unit) const
{
	return ViAudioPosition::convertPosition(mStartIndex + mSize - 1, ViAudioPosition::Samples, unit, mFormat);
}

qreal ViNoiseWindow::length(const ViAudioPosition::Unit &unit) const
{
	return ViAudioPosition::convertPosition(mSize, ViAudioPosition::Samples, unit, mFormat);
}

