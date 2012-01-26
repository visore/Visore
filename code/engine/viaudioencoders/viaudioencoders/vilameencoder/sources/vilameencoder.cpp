#include "vilameencoder.h"

//ViLameEncoder* ViLameEncoder::mInstance = NULL;

ViLameEncoder::ViLameEncoder()
	: ViAudioEncoder()
{

}

ViAudioEncoder* ViLameEncoder::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViLameEncoder>(new ViLameEncoder).dynamicCast<ViAudioEncoder>();
	}
	return mInstance.data();
}
