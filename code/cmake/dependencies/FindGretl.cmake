# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# FIXME: How do I find the version of GRETL that I want to use?
# What versions are available?

# NOTE: GRETL prefix is understood to be the path to the root of the GRETL
# installation library.
set(GRETL_PREFIX "" CACHE PATH "The path to the prefix of a GRETL installation")

set(GRETL_INCLUDE_PATHS
	${GRETL_PREFIX}/include
	/usr/include
	/usr/local/include
	/usr/include/gretl 
)

set(GRETL_LIBRARY_PATHS
	${GRETL_PREFIX}/lib
	/usr/lib
	/usr/local/lib
	/usr/lib/gretl-gtk2
)

find_path(GRETL_INCLUDE_DIR libgretl.h PATHS ${GRETL_INCLUDE_PATHS} )

find_library(GRETL_LIBRARY NAMES libgretl.so libgretl-1.0.so PATHS ${GRETL_LIBRARY_PATHS} )

if(GRETL_INCLUDE_DIR AND GRETL_LIBRARY)
	set(GRETL_FOUND TRUE)
endif()

if(GRETL_FOUND)
   if(NOT GRETL_FIND_QUIETLY)
	  MESSAGE(STATUS "Found GRETL: ${GRETL_LIBRARY}")
   endif()
elseif(GRETL_FOUND)
   if(GRETL_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find GRETL")
   endif()
endif()

