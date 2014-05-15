#include <vigretl.h>

bool ViGretl::mInitialized = false;

bool ViGretl::initialize(const bool &enableOutput)
{
	if(mInitialized) return false;
	mInitialized = true;
	libgretl_init();
	//if(!enableOutput) freopen("/dev/null", "w", stderr); // Stop forced gretl error output
	return mInitialized;
}

bool ViGretl::uninitialize()
{
	if(!mInitialized) return false;
	mInitialized = false;
	libgretl_cleanup();
	return mInitialized;
}
