#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_ALIGNER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_ALIGNER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vialigner SHARED ${VI_ALIGNER_HEADERS} ${VI_ALIGNER_SOURCES})
TARGET_LINK_LIBRARIES(vialigner viaudiobuffer)
QT5_USE_MODULES(vialigner Multimedia)

#Install file
INSTALL(TARGETS vialigner DESTINATION engine)
