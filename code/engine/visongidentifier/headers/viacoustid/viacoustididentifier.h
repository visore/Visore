#ifndef VIACOUSTIDIDENTIFIER_H
#define VIACOUSTIDIDENTIFIER_H

#include <viacoustidfingerprinter.h>
#include <visongidentifier.h>

class ViAcoustidIdentifier : public ViSongIdentifier
{

	Q_OBJECT

	private slots:

		void sendRequest();
		void processReply(bool success);

	public:

		ViAcoustidIdentifier();

		void identifyTrack(ViBufferOffsets bufferOffset);

	private:

		ViAcoustidFingerprinter mFingerprinter;

};

#endif
