#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_NEAREST_NEIGHBOUR_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_NEAREST_NEIGHBOUR_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vinearestneighbournoisedetector SHARED ${VI_NEAREST_NEIGHBOUR_NOISE_DETECTOR_HEADERS} ${VI_NEAREST_NEIGHBOUR_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vinearestneighbournoisedetector vinoisedetector)
QT5_USE_MODULES(vinearestneighbournoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vinearestneighbournoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
