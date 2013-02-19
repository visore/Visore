#include "vivulcanotheme.h"
#include "vivulcanothemecolors.h"
#include "vivulcanothemefonts.h"

ViVulcanoTheme::ViVulcanoTheme()
	: ViTheme()
{
	mName = "Vulcano";
	mColors = new ViVulcanoThemeColors();
	mFonts = new ViVulcanoThemeFonts();
}

#ifdef __cplusplus
extern "C"
{
#endif

ViThemePointer createTheme()
{
   return ViVulcanoTheme::instance();
}

#ifdef __cplusplus
}
#endif
