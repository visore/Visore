#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_PROCESSINGCHAIN_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp ${SOURCES}/*/*/*.cpp)
FILE(GLOB VI_PROCESSINGCHAIN_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h ${HEADERS}/*/*/*.h)

#Create library
ADD_LIBRARY(viprocessingchain SHARED ${VI_PROCESSINGCHAIN_HEADERS} ${VI_PROCESSINGCHAIN_SOURCES})
TARGET_LINK_LIBRARIES(viprocessingchain viaudioobject viaudioconnection vifouriertransformer visongidentifier viproject)

#Install file
INSTALL(TARGETS viprocessingchain DESTINATION engine)
