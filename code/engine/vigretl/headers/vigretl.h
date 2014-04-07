#ifndef VIGRETL_H
#define VIGRETL_H

#include <libintl.h> // Very important. Solves the unqualified-id problem in libintl.h. We include it before gretl does
#include <gretl/libgretl.h>
#include <gretl/forecast.h>

// Declared in desribe.c in gretl. Since it is not in describe.h, we manually have to declare it
struct MahalDist_ {
	int *list;
	int n;
	double *d;
};

extern "C" int libset_set_int (const char *key, int val); // Not defined as extern in gretl. We have to do it manually
#include <gretl/libset.h>

class ViGretl
{

	public:

		static bool initialize(const bool &enableOutput = false);	// Initialize on program start
		static bool uninitialize(); // Uninitialize on program exit

	private:

		static bool mInitialized;

};

#endif
