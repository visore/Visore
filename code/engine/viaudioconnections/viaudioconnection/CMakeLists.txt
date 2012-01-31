#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Include common headers
INCLUDE_DIRECTORIES(${HEADERS})

#Initiate source files
FILE(GLOB VI_AUDIOCONNECTION_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOCONNECTION_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudioconnection SHARED ${VI_AUDIOCONNECTION_HEADERS} ${VI_AUDIOCONNECTION_SOURCES})
TARGET_LINK_LIBRARIES(viaudioconnection ${QT_LIBRARIES} viaudioencoder viaudioformat viaudiobuffer)

#Install file
INSTALL(TARGETS viaudioconnection DESTINATION engine)
