#ifndef VICUSTOMMASKCREATOR_H
#define VICUSTOMMASKCREATOR_H

#include <vinotifier.h>
#include <viaudiodata.h>

class ViCustomMaskCreator : public ViNotifier
{

	public:

		ViCustomMaskCreator();
		virtual ~ViCustomMaskCreator();

		void clear();

		void setBuffers(ViBuffer *read1, ViBuffer *read2, ViBuffer *write1, ViBuffer *write2);

		void create();

	private:

		ViAudioReadData mRead1;
		ViAudioReadData mRead2;
		ViAudioWriteData mWrite1;
		ViAudioWriteData mWrite2;

};

#endif
