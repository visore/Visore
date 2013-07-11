#ifndef VINOISEWINDOW_H
#define VINOISEWINDOW_H

#include <viaudioposition.h>
#include <viaudioformat.h>

class ViNoiseWindow
{

	public:

		ViNoiseWindow();
		ViNoiseWindow(const int &startIndex, const int &size, const int &channel, const ViAudioFormat &format);
		ViNoiseWindow(const ViNoiseWindow &other);

		void setStartIndex(const int &startIndex);
		int startIndex() const;

		void setSize(const int &size);
		int size() const;

		void setChannel(const int &channel);
		int channel() const;

		void setFormat(const ViAudioFormat &format);
		ViAudioFormat format() const;

		qreal from(const ViAudioPosition::Unit &unit) const;
		qreal to(const ViAudioPosition::Unit &unit) const;
		qreal length(const ViAudioPosition::Unit &unit) const;

	private:

		int mStartIndex;
		int mSize;
		int mChannel;
		ViAudioFormat mFormat;

};

#endif
