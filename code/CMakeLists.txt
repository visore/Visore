#Setup Visore
PROJECT(Visore)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
CMAKE_POLICY(SET CMP0012 NEW) #Ensures that TRUE can be used as a boolean
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fpermissive") # -fpermissive for Echonest that includes Boost
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x") # Add C++11 support
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings") # String to char* warnings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include") # For gretl to use glib. After configureing gretl, type the command (inside config dir): pkg-config --cflags glib-2.0   and use the output


#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(MAIN ${ROOT}/main)
SET(LAUNCHER ${ROOT}/launcher)
SET(ENGINE ${ROOT}/engine CACHE INTERNAL "Engine directory")
SET(INTERFACE ${ROOT}/interface CACHE INTERNAL "Interface directory")
SET(EXTERNAL ${ROOT}/external CACHE INTERNAL "External directory")
SET(ESSENTIALS ${ROOT}/essentials CACHE INTERNAL "Essentials directory")
SET(COMMON ${ROOT}/common CACHE INTERNAL "Common directory")
SET(BENCHMARKER ${ROOT}/benchmarker CACHE INTERNAL "Benchmarker directory")

#Setup CMake
SET(CMAKE_MODULE_PATH ${ROOT}/cmake)
INCLUDE(Version)
INCLUDE(Dependencies)
INCLUDE(Macros)

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

#Create install directory structure
IF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL "")
	FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX})
ENDIF()

#Setup Qt5
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(QT5_DIR "/opt/Qt5.2.0/5.2.0/gcc_64" CACHE PATH "Qt5 install location (containing the bin, include and lib directories - eg: /usr/local/Qt-5.0.0)")
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_DIR})
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Network)
FIND_PACKAGE(Qt5Xml)
FIND_PACKAGE(Qt5Multimedia)

FIND_PACKAGE(Gretl)

#Set flags for Windows and MinGW auto-import
IF(WIN32)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--enable-auto-import")
	SET(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -Wl,--enable-auto-import")
ENDIF()

#Include common headers
FILE_DIRECTORIES(GLOBAL_FILES)
INCLUDE_DIRECTORIES(${GLOBAL_FILES})

#Initiate sub-directories
ADD_SUBDIRECTORY(${COMMON})
ADD_SUBDIRECTORY(${EXTERNAL})
ADD_SUBDIRECTORY(${ESSENTIALS})
ADD_SUBDIRECTORY(${ENGINE})
ADD_SUBDIRECTORY(${INTERFACE})
ADD_SUBDIRECTORY(${MAIN})
ADD_SUBDIRECTORY(${LAUNCHER})
ADD_SUBDIRECTORY(${BENCHMARKER})

INCLUDE(Packaging)
