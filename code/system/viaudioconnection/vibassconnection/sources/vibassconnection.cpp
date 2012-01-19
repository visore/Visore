#include "vibassconnection.h"

/*******************************************
ViBassFileInput
********************************************/

#ifdef __cplusplus
extern "C"
{
#endif

ViFileInput* createFileInput()
{
   return new ViBassFileInput();
}

#ifdef __cplusplus
}
#endif

/*******************************************
ViBassStreamInput
********************************************/
