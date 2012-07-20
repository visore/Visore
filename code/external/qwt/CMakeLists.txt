#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB QWT_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB QWT_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(qwt SHARED ${QWT_SOURCES} ${QWT_HEADERS})
QT5_USE_MODULES(qwt Core Gui PrintSupport Concurrent Svg)

#Install file
INSTALL(TARGETS qwt DESTINATION external)
