#ifndef VIAUDIOPROCESSINGCHAIN_H
#define VIAUDIOPROCESSINGCHAIN_H

#include "viaudiobuffer.h"
#include "viaudioinput.h"
#include "viaudiooutput.h"

class ViAudioProcessingChain : public QObject, public ViError
{
    Q_OBJECT

	private slots:
		void originalBufferChanged(int startIndex, int size);
		void correctedBufferChanged(int startIndex, int size);

	public:
		ViAudioProcessingChain();
		~ViAudioProcessingChain();

		ViAudioBuffer* originalBuffer();
		ViAudioBuffer* correctedBuffer();

		void attachInput(ViAudioInput *input);
		void attachStreamOutput(ViAudioOutput *output);
		void attachFileOutput(ViAudioOutput *output);

		void reset();

	private:
		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
		ViAudioBufferStream *mOriginalReadStream;
		ViAudioBufferStream *mCorrectedWriteStream;

		ViAudioInput *mInput;
		ViAudioOutput *mStreamOutput;
		ViAudioOutput *mFileOutput;
};

#endif
