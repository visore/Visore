#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_GARCH_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_GARCH_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vigarchpredictor SHARED ${VI_GARCH_PREDICTOR_HEADERS} ${VI_GARCH_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(vigarchpredictor vipredictor vigretl)
QT5_USE_MODULES(vigarchpredictor Multimedia)

#Install file
INSTALL(TARGETS vigarchpredictor DESTINATION ${VI_PREDICTORS_INSTALL_PATH})
