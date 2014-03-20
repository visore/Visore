#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB FANN_SOURCES ${SOURCES}/doublefann.c)
FILE(GLOB FANN_HEADERS ${HEADERS}/*.h ${HEADERS}/*.hpp)

#Create library
ADD_LIBRARY(fann SHARED ${FANN_SOURCES} ${FANN_HEADERS})

#Install file
INSTALL(TARGETS fann DESTINATION external)
