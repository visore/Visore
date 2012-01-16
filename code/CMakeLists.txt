#Setup Visore
PROJECT(Visore)

#Make a debug version. Remove this statement to remove all debug outputs
ADD_DEFINITIONS(-DDEBUG)

#Setup CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(MAIN ${ROOT}/main)
SET(SYSTEM ${ROOT}/system)
SET(INTERFACE ${ROOT}/interface)
SET(EXTERNAL ${ROOT}/external)
SET(ESSENTIALS ${ROOT}/essentials)

#Set the module path
set(CMAKE_MODULE_PATH ${ROOT}/cmake)

#Setup Qt
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(QtMobility REQUIRED)

#Include common headers
INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${QTMOBILITY_INCLUDE_DIRS}
	${EXTERNAL}/fftreal/headers
	${ESSENTIALS}
	${SYSTEM}/viaudioengine/headers
	${INTERFACE}/vimainwindow/headers
)

#Unix version for installing shared libraries and linking them to the executable
SET(CMAKE_INSTALL_RPATH
	"${CMAKE_INSTALL_PREFIX}/system"
	"${CMAKE_INSTALL_PREFIX}/external"
	"${CMAKE_INSTALL_PREFIX}/interface"
)

#Initiate sub-directories
ADD_SUBDIRECTORY(${EXTERNAL})
ADD_SUBDIRECTORY(${SYSTEM})
ADD_SUBDIRECTORY(${INTERFACE})
ADD_SUBDIRECTORY(${MAIN})
