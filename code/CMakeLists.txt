#Setup Visore
PROJECT(Visore)

#Setup CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(MAIN ${ROOT}/main)
SET(LAUNCHER ${ROOT}/launcher)
SET(ENGINE ${ROOT}/engine CACHE INTERNAL "Engine directory")
SET(INTERFACE ${ROOT}/interface CACHE INTERNAL "Interface directory")
SET(EXTERNAL ${ROOT}/external CACHE INTERNAL "External directory")
SET(ESSENTIALS ${ROOT}/essentials CACHE INTERNAL "Essentials directory")
SET(COMMON ${ROOT}/common CACHE INTERNAL "Common directory")

#Setup CMake
SET(CMAKE_MODULE_PATH ${ROOT}/cmake)
INCLUDE(Version)
INCLUDE(Dependencies)

#Set System
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(MACOSX 1)
	ADD_DEFINITIONS(-DMACOSX)
	SET(SYSTEM "MACOSX" CACHE INTERNAL "Current system")
    SET(DISTRO " " CACHE INTERNAL "Linux distrobution")    
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	ADD_DEFINITIONS(-DLINUX)
	SET(SYSTEM "LINUX" CACHE INTERNAL "Current system")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	ADD_DEFINITIONS(-DWINDOWS)
	SET(SYSTEM "WINDOWS" CACHE INTERNAL "Current system")
    SET(DISTRO " " CACHE INTERNAL "Linux distrobution")
ENDIF()

#Setup Qt5
FIND_PACKAGE(Qt5 COMPONENTS Multimedia Core Widgets Gui Network PrintSupport Concurrent Svg)

#Setup FFMPEG
FIND_PACKAGE(Ffmpeg)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS")

#Set flags for Windows and MinGW auto-import
IF(WIN32)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--enable-auto-import")
	SET(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -Wl,--enable-auto-import")
ENDIF()

#Include common headers
INCLUDE_DIRECTORIES(
	
	${CMAKE_BINARY_DIR}
	${ESSENTIALS}/headers

	${INTERFACE}/vimainwindow/headers
	${INTERFACE}/vigradientcreator/headers
	${INTERFACE}/viwidgets/viwidget/headers
	${INTERFACE}/vithemes/vitheme/headers
	${INTERFACE}/visimplewidgets/headers
	${INTERFACE}/vicomposedwidgets/headers
	${INTERFACE}/vicomposedwidgets/headers/vispectrumplot

	${ENGINE}/viaudioengine/headers
	${ENGINE}/viaudioconnections/viaudioconnection/headers
	${ENGINE}/viaudioformats/viaudioformat/headers
	${ENGINE}/viaudiobuffer/headers
	${ENGINE}/vispectrumanalyzer/headers
	${ENGINE}/viwaveformer/headers
	${ENGINE}/vicoder/headers
	${ENGINE}/visongdetector/headers
	${ENGINE}/vifouriertransformer/headers
	${ENGINE}/vimatcher/headers
	${ENGINE}/vispectrumanalyzer/headers

	${EXTERNAL}/fftreal/sources
	${EXTERNAL}/fftreal/headers
	${EXTERNAL}/qplotting/headers
	${EXTERNAL}/qwt/headers
)

IF(WIN32)
	INCLUDE_DIRECTORIES(${EXTERNAL}/dlfcn/headers)
ENDIF()

#Initiate sub-directories
ADD_SUBDIRECTORY(${EXTERNAL})
ADD_SUBDIRECTORY(${ENGINE})
ADD_SUBDIRECTORY(${INTERFACE})
ADD_SUBDIRECTORY(${ESSENTIALS})
ADD_SUBDIRECTORY(${MAIN})
ADD_SUBDIRECTORY(${LAUNCHER})
ADD_SUBDIRECTORY(${COMMON})

INCLUDE(Packaging)
