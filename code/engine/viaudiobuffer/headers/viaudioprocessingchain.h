#ifndef VIAUDIOPROCESSINGCHAIN_H
#define VIAUDIOPROCESSINGCHAIN_H

#include "viaudiobuffer.h"
#include "viaudioinput.h"
#include "viaudiooutput.h"
#include "viprocessor.h"
#include "viprocessorlist.h"

class ViAudioProcessingChain : public QObject, public ViError
{
    Q_OBJECT

	private slots:
		void originalBufferChanged(int size, int id);
		void correctedBufferChanged(int size, int id);

	public:
		ViAudioProcessingChain();
		~ViAudioProcessingChain();

		ViAudioBuffer* originalBuffer();
		ViAudioBuffer* correctedBuffer();

		void attachInput(ViAudioInput *input);
		void attachStreamOutput(ViAudioOutput *output);
		void attachFileOutput(ViAudioOutput *output);
		void attachOriginalProcessor(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution);
		void attachCorrectedProcessor(ViProcessor *processor, ViProcessorList::ViProcessorExecution execution);

		void reset();

	private:
		ViAudioBuffer *mOriginalBuffer;
		ViAudioBuffer *mCorrectedBuffer;
		ViAudioBufferStream *mOriginalReadStream;
		ViAudioBufferStream *mCorrectedWriteStream;

		ViAudioInput *mInput;
		ViAudioOutput *mStreamOutput;
		ViAudioOutput *mFileOutput;

		ViProcessorList mOriginalProcessors;
		ViProcessorList mCorrectedProcessors;
};

#endif
