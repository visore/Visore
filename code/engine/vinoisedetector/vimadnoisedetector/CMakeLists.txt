#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MAD_NOISE_DETECTOR_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_MAD_NOISE_DETECTOR_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vimadnoisedetector SHARED ${VI_MAD_NOISE_DETECTOR_HEADERS} ${VI_MAD_NOISE_DETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(vimadnoisedetector vinoisedetector)
QT5_USE_MODULES(vimadnoisedetector Multimedia Xml)

#Install file
INSTALL(TARGETS vimadnoisedetector DESTINATION ${VI_NOISE_DETECTOR_INSTALL_PATH})
