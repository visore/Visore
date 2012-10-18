#ifndef VIPLAYBACKHANDLER_H
#define VIPLAYBACKHANDLER_H

#include "vihandler.h"

class ViPlaybackHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void changeOutput(ViStreamOutput *output);
		void update(ViAudioObjectPointer object);
		void start();

	public:

		ViPlaybackHandler(ViProcessingChain *chain);
		~ViPlaybackHandler();

		ViAudioObjectPointer currentObject();

	private:

		ViStreamOutput *mOutput;
		ViAudioObjectQueue mObjects;
		bool mWaitingForFormat;
		ViAudioObjectPointer mCurrentObject;

};

#endif
