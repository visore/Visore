#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIOBUFFER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOBUFFER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudiobuffer SHARED ${VI_AUDIOBUFFER_HEADERS} ${VI_AUDIOBUFFER_SOURCES})
TARGET_LINK_LIBRARIES(viaudiobuffer viessentials)
QT5_USE_MODULES(viaudiobuffer Multimedia)

#Install file
INSTALL(TARGETS viaudiobuffer DESTINATION engine)
