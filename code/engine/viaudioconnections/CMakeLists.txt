#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIOCONNECTION_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOCONNECTION_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viaudioconnection SHARED ${VI_AUDIOCONNECTION_HEADERS} ${VI_AUDIOCONNECTION_SOURCES})
TARGET_LINK_LIBRARIES(viaudioconnection viaudiocoder viaudiobuffer)
QT5_USE_MODULES(viaudioconnection Multimedia)

#Install file
INSTALL(TARGETS viaudioconnection DESTINATION engine)
