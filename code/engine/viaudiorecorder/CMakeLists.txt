#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIORECORDER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIORECORDER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudiorecorder SHARED ${VI_AUDIORECORDER_HEADERS} ${VI_AUDIORECORDER_SOURCES})
TARGET_LINK_LIBRARIES(viaudiorecorder viaudioconnection viproject viprocessor)
QT5_USE_MODULES(viaudiorecorder Multimedia Xml)

#Install file
INSTALL(TARGETS viaudiorecorder DESTINATION engine)
