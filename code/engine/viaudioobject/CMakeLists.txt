#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIOOBJECT_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOOBJECT_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudioobject SHARED ${VI_AUDIOOBJECT_HEADERS} ${VI_AUDIOOBJECT_SOURCES})
TARGET_LINK_LIBRARIES(viaudioobject viaudiobuffer)
QT5_USE_MODULES(viaudioobject Multimedia Xml)

#Install file
INSTALL(TARGETS viaudioobject DESTINATION engine)
