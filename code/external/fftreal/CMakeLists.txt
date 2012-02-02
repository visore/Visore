#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Include sources
INCLUDE_DIRECTORIES(${HEADERS} ${SOURCES}/*.cpp)

#Initiate source files
FILE(GLOB FFTREAL_HEADERS ${HEADERS}/*.h)
FILE(GLOB FFTREAL_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(fftreal SHARED ${FFTREAL_HEADERS} ${FFTREAL_SOURCES})

#Install file
INSTALL(TARGETS fftreal DESTINATION external/fftreal)
