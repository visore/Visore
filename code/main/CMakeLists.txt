#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MAIN_SOURCES ${SOURCES}/*.cpp)

#Create executable
SET(ISWINRELEASE 0)
IF(CMAKE_BUILD_TYPE STREQUAL "Release")
	IF(WIN32)
		SET(ISWINRELEASE 1)
	ENDIF()
ENDIF()

IF(${ISWINRELEASE})
	#Ensures that the console is not shown in Windows
	ADD_EXECUTABLE(visore WIN32 ${VI_MAIN_SOURCES})
ELSE()
	ADD_EXECUTABLE(visore ${VI_MAIN_SOURCES})
ENDIF()

#Link Visore to Qt
TARGET_LINK_LIBRARIES(visore viaudioengine vimainwindow vilinearpredictor)
QT5_USE_MODULES(visore Widgets Multimedia Xml)

#Install file
INSTALL(TARGETS visore RUNTIME DESTINATION .)
