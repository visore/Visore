#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIOENGINE_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOENGINE_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudioengine SHARED ${VI_AUDIOENGINE_HEADERS} ${VI_AUDIOENGINE_SOURCES})
TARGET_LINK_LIBRARIES(viaudioengine viessentials viaudiorecorder vialigner vicorrelator)
QT5_USE_MODULES(viaudioengine Multimedia Xml)

#Install file
INSTALL(TARGETS viaudioengine DESTINATION engine)
