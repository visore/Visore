/*****************************************************************************

        FFTRealSelect.hpp
        By Laurent de Soras

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (ffft_FFTRealSelect_CURRENT_CODEHEADER)
	#error Recursive inclusion of FFTRealSelect code header.
#endif
#define	ffft_FFTRealSelect_CURRENT_CODEHEADER

#if ! defined (ffft_FFTRealSelect_CODEHEADER_INCLUDED)
#define	ffft_FFTRealSelect_CODEHEADER_INCLUDED

//#include	"FFTRealSelect.h"

namespace ffft
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int P>
double *	FFTRealSelect <P>::sel_bin (double *e_ptr, double *o_ptr)
{
	return (o_ptr);
}



template <>
inline double *	FFTRealSelect <0>::sel_bin (double *e_ptr, double *o_ptr)
{
	return (e_ptr);
}



}	// namespace ffft



#endif	// ffft_FFTRealSelect_CODEHEADER_INCLUDED

#undef ffft_FFTRealSelect_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
