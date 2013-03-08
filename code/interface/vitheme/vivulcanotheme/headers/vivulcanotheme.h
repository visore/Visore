#ifndef VIVULCANOTHEME_H
#define VIVULCANOTHEME_H

#include "vitheme.h"

class ViVulcanoTheme : public ViTheme, public ViSingleton<ViVulcanoTheme>
{

	friend class ViSingleton<ViVulcanoTheme>;

	protected:

		ViVulcanoTheme();
};

#endif
