#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_PREDICTORS_INSTALL_PATH engine/predictors)

#Add definitions
ADD_DEFINITIONS(-DPREDICTORS_PATH="${VI_PREDICTORS_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vipolynomialpredictor)
ADD_SUBDIRECTORY(${ROOT}/vihermitepredictor)
ADD_SUBDIRECTORY(${ROOT}/viarmapredictor)
ADD_SUBDIRECTORY(${ROOT}/vigarchpredictor)
ADD_SUBDIRECTORY(${ROOT}/vifourierpredictor)
ADD_SUBDIRECTORY(${ROOT}/viconstantpredictor)
ADD_SUBDIRECTORY(${ROOT}/vilagrangepredictor)
ADD_SUBDIRECTORY(${ROOT}/vinewtonpredictor)

#Initiate source files
FILE(GLOB VI_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vipredictor SHARED ${VI_PREDICTOR_HEADERS} ${VI_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(vipredictor viessentials)
QT5_USE_MODULES(vipredictor Xml Multimedia)

#Install file
INSTALL(TARGETS vipredictor DESTINATION engine)
