#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_FOURIER_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_FOURIER_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vifouriernoisedetector SHARED ${VI_FOURIER_NOISE_DETECTOR_HEADERS} ${VI_FOURIER_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vifouriernoisedetector vinoisedetector)
QT5_USE_MODULES(vifouriernoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vifouriernoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
