#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_CONSTANT_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_CONSTANT_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viconstantpredictor SHARED ${VI_CONSTANT_PREDICTOR_HEADERS} ${VI_CONSTANT_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(viconstantpredictor vipredictor)
QT5_USE_MODULES(viconstantpredictor Multimedia)

#Install file
INSTALL(TARGETS viconstantpredictor DESTINATION ${VI_PREDICTORS_INSTALL_PATH})
