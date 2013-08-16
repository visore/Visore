#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_PREDICTOR_INSTALL_PATH engine/predictors)

#Add definitions
ADD_DEFINITIONS(-DPREDICTOR_PATH="${VI_PREDICTOR_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vilinearpredictor)

#Initiate source files
FILE(GLOB VI_PREDICTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_PREDICTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vipredictor SHARED ${VI_PREDICTOR_HEADERS} ${VI_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(vipredictor viessentials)
QT5_USE_MODULES(vipredictor Multimedia Xml)

#Install file
INSTALL(TARGETS vipredictor DESTINATION ${VI_PREDICTOR_INSTALL_PATH})
