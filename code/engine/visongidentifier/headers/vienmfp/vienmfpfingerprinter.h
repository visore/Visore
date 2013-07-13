#ifndef VIENMFPFINGERPRINTER_H
#define VIENMFPFINGERPRINTER_H

#include <vifingerprinter.h>
#include <viaudiocoder.h>

class ViEnmfpFingerprinterThread : public ViFingerprinterThread
{

	public:

		ViEnmfpFingerprinterThread();
		void run();

};

class ViEnmfpFingerprinter : public ViFingerprinter
{

	Q_OBJECT

	private slots:

		void generate();

	public:

		ViEnmfpFingerprinter();
		void generate(ViBufferOffsets bufferOffset);
		QString version();

	protected:

		ViAudioFormat encodingFormat();

	private:

		QString mVersion;
		ViAudioCoder mCoder;
		ViBuffer mOutput;

};

#endif
