set(GRETL_PREFIX "" CACHE PATH "The path to the prefix of a GRETL installation")

set(GRETL_INCLUDE_PATHS
	${GRETL_PREFIX}/include
	/usr/include
	/usr/local/include
	/usr/include/gretl 
	/usr/local/include/gretl 
)

set(GRETL_LIBRARY_PATHS
	${GRETL_PREFIX}/lib
	/usr/lib
	/usr/local/lib
	/usr/lib/gretl-gtk2
	/usr/local/lib/gretl-gtk2
)

find_path(GRETL_INCLUDE_DIR libgretl.h PATHS ${GRETL_INCLUDE_PATHS} )

find_library(GRETL_LIBRARY NAMES libgretl.so libgretl-1.0.so PATHS ${GRETL_LIBRARY_PATHS} )
find_library(GRETL_ARMA_LIBRARY NAMES arma.so PATHS ${GRETL_LIBRARY_PATHS} ) # Manually link to arma.so, otherwise we constantly load and unload the library dynamically (done through libgretl)

if(GRETL_INCLUDE_DIR AND GRETL_LIBRARY)
	set(GRETL_FOUND TRUE)
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # Fixes linker errors with arma.so
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

