#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_ARMA_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_ARMA_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viarmapredictor SHARED ${VI_ARMA_PREDICTOR_HEADERS} ${VI_ARMA_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(viarmapredictor vipredictor vigretl)
QT5_USE_MODULES(viarmapredictor Multimedia)

#Install file
INSTALL(TARGETS viarmapredictor DESTINATION ${VI_PREDICTORS_INSTALL_PATH})
