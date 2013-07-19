#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIO_PLAYER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIO_PLAYER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudioplayer SHARED ${VI_AUDIO_PLAYER_HEADERS} ${VI_AUDIO_PLAYER_SOURCES})
TARGET_LINK_LIBRARIES(viaudioplayer viaudioconnection viaudioobject)
QT5_USE_MODULES(viaudioplayer Multimedia Xml)

#Install file
INSTALL(TARGETS viaudioplayer DESTINATION engine)
