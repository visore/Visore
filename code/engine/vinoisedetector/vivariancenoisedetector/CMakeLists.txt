#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_FREQUENCY_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_FREQUENCY_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vifrequencynoisedetector SHARED ${VI_FREQUENCY_NOISE_DETECTOR_HEADERS} ${VI_FREQUENCY_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vifrequencynoisedetector vinoisedetector)
QT5_USE_MODULES(vifrequencynoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vifrequencynoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
