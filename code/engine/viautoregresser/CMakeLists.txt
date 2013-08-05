#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUTO_REGRESSER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_AUTO_REGRESSER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(viautoregresser SHARED ${VI_AUTO_REGRESSER_HEADERS} ${VI_AUTO_REGRESSER_SOURCES})
TARGET_LINK_LIBRARIES(viautoregresser viessentials)
QT5_USE_MODULES(viautoregresser Multimedia Xml)

#Install file
INSTALL(TARGETS viautoregresser DESTINATION engine)
