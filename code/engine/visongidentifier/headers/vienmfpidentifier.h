#ifndef VIENMFPIDENTIFIER_H
#define VIENMFPIDENTIFIER_H

#include "viechoidentifier.h"

class ViEnmfpCodeThread : public ViEchoCodeThread
{

	protected:

		void generateCode(const QByteArray *data, int offset, QString &code, QString &version, int &codeLength);

};

class ViEnmfpIdentifier : public ViEchoIdentifier
{

	protected:

		ViEchoCodeThread* codeThread();
		ViAudioFormat encodingFormat();

};

#endif
