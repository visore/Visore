#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_NEWTON_PREDICTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_NEWTON_PREDICTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vinewtonpredictor SHARED ${VI_NEWTON_PREDICTOR_HEADERS} ${VI_NEWTON_PREDICTOR_SOURCES})
TARGET_LINK_LIBRARIES(vinewtonpredictor vipredictor)
QT5_USE_MODULES(vinewtonpredictor Xml Multimedia)

#Install file
INSTALL(TARGETS vinewtonpredictor DESTINATION ${VI_PREDICTORS_INSTALL_PATH})
