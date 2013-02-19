#ifndef VIENMFPIDENTIFIER_H
#define VIENMFPIDENTIFIER_H

#include <vienmfpfingerprinter.h>
#include <visongidentifier.h>

class ViEnmfpIdentifier : public ViSongIdentifier
{

	Q_OBJECT

	private slots:

		void sendRequest();
		void processReply(bool success);

	public:

		ViEnmfpIdentifier();
		void identify(ViBuffer *buffer);

	private:

		ViEnmfpFingerprinter mFingerprinter;

};

#endif
