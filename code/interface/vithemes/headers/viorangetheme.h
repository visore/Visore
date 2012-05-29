#ifndef VIORANGETHEME_H
#define VIORANGETHEME_H

#include "vitheme.h"
#include "viorangethemecolors.h"

class ViOrangeTheme : public ViTheme
{
	public:
		static ViTheme* instance();

	protected:
		ViOrangeTheme();
};

#endif
