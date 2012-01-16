#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Include sources
INCLUDE_DIRECTORIES(${HEADERS}/fftreal ${HEADERS} ${SOURCES}/fftreal)

#Initiate source files
FILE(GLOB VI_FFTREAL_HEADERS ${HEADERS}/*.h ${HEADERS}/fftreal/*.h)
FILE(GLOB VI_FFTREAL_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vifftreal SHARED ${VI_FFTREAL_SOURCES})

#Install file
INSTALL(TARGETS vifftreal DESTINATION external)
