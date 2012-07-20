#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB OSDAB_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB OSDAB_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(osdab SHARED ${OSDAB_SOURCES} ${OSDAB_HEADERS})
QT5_USE_MODULES(osdab Core Gui PrintSupport Concurrent OpenGL)

#Install file
INSTALL(TARGETS osdab DESTINATION external)
