#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MAHALANOBIS_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_MAHALANOBIS_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vimahalanobisnoisedetector SHARED ${VI_MAHALANOBIS_NOISE_DETECTOR_HEADERS} ${VI_MAHALANOBIS_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vimahalanobisnoisedetector vinoisedetector)
QT5_USE_MODULES(vimahalanobisnoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vimahalanobisnoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
