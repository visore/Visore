#Setup Visore
PROJECT(Visore)

#Make a debug version. Remove this statement to remove all debug outputs
ADD_DEFINITIONS(-DDEBUG)

#Setup CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(MAIN ${ROOT}/main)
SET(ENGINE ${ROOT}/engine CACHE INTERNAL "Engine directory")
SET(INTERFACE ${ROOT}/interface CACHE INTERNAL "Interface directory")
SET(EXTERNAL ${ROOT}/external CACHE INTERNAL "External directory")
SET(ESSENTIALS ${ROOT}/essentials CACHE INTERNAL "Essentials directory")

#Set the module path
SET(CMAKE_MODULE_PATH ${ROOT}/cmake)

#Setup Qt
FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(QtMobility REQUIRED)

#Include common headers
INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${QTMOBILITY_INCLUDE_DIRS}
	${ESSENTIALS}/headers
	${INTERFACE}/vimainwindow/headers
	${ENGINE}/viaudioengine/headers
	${ENGINE}/viaudioconnections/viaudioconnection/headers
	${ENGINE}/viaudioformats/viaudioformat/headers
)

#Unix version for installing shared libraries and linking them to the executable
SET(CMAKE_INSTALL_RPATH
	"${CMAKE_INSTALL_PREFIX}/essentials"
	"${CMAKE_INSTALL_PREFIX}/engine"
	"${CMAKE_INSTALL_PREFIX}/engine/connections"
	"${CMAKE_INSTALL_PREFIX}/engine/formats"
	"${CMAKE_INSTALL_PREFIX}/interface"
)

#Initiate sub-directories
ADD_SUBDIRECTORY(${EXTERNAL})
ADD_SUBDIRECTORY(${ENGINE})
ADD_SUBDIRECTORY(${INTERFACE})
ADD_SUBDIRECTORY(${MAIN})
ADD_SUBDIRECTORY(${ESSENTIALS})
