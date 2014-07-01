#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_NOISE_DETECTOR_INSTALL_PATH engine/noisedetectors)

#Add definitions
ADD_DEFINITIONS(-DNOISE_DETECTOR_PATH="${VI_NOISE_DETECTOR_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vizscorenoisedetector)
ADD_SUBDIRECTORY(${ROOT}/vimahalanobisnoisedetector)
ADD_SUBDIRECTORY(${ROOT}/vinearestneighbournoisedetector)
ADD_SUBDIRECTORY(${ROOT}/vipredictionnoisedetector)
ADD_SUBDIRECTORY(${ROOT}/vifouriernoisedetector)
ADD_SUBDIRECTORY(${ROOT}/vimadnoisedetector)

#Initiate source files
FILE(GLOB VI_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vinoisedetector SHARED ${VI_NOISE_DETECTOR_HEADERS} ${VI_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vinoisedetector viaudiobuffer)
QT5_USE_MODULES(vinoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vinoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
