#ifndef VIPLAYBACKHANDLER_H
#define VIPLAYBACKHANDLER_H

#include "vihandler.h"

class ViPlaybackHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void changeOutput(ViStreamOutput *output);
		void update(ViAudioObject *object);
		void start();

	public:

		ViPlaybackHandler(ViProcessingChain *chain);
		~ViPlaybackHandler();

		ViAudioObject* currentObject();

	private:

		ViStreamOutput *mOutput;
		ViAudioObjectQueue mObjects;
		bool mWaitingForFormat;
		ViAudioObject *mCurrentObject;

};

#endif
