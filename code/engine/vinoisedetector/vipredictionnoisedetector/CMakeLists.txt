#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_PREDICTION_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_PREDICTION_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vipredictionnoisedetector SHARED ${VI_PREDICTION_NOISE_DETECTOR_HEADERS} ${VI_PREDICTION_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vipredictionnoisedetector vinoisedetector)
QT5_USE_MODULES(vipredictionnoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vipredictionnoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
