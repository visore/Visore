#ifndef VIAUDIOBUFFERCHUNK_H
#define VIAUDIOBUFFERCHUNK_H

#include <QString>

class ViAudioBufferChunk
{
	public:
		ViAudioBufferChunk();
		ViAudioBufferChunk(char *data);
		ViAudioBufferChunk(int size);
		~ViAudioBufferChunk();
		char* data();
		void setData(char *data);

	private:
		char *mData;
};

#endif
