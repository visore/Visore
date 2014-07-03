#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_NEURAL_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_NEURAL_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vineuralpredictor SHARED ${VI_NEURAL_PREDICTOR_HEADERS} ${VI_NEURAL_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(vineuralpredictor vipredictor vifann)
QT5_USE_MODULES(vineuralpredictor Multimedia)

#Install file
INSTALL(TARGETS vineuralpredictor DESTINATION ${VI_PREDICTORS_INSTALL_PATH})
