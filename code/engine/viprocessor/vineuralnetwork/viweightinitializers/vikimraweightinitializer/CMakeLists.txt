#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_KIM_RA_WEIGHT_INITIALIZER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_KIM_RA_WEIGHT_INITIALIZER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vikimraweightinitializer SHARED ${VI_KIM_RA_WEIGHT_INITIALIZER_HEADERS} ${VI_KIM_RA_WEIGHT_INITIALIZER_SOURCES})
TARGET_LINK_LIBRARIES(vikimraweightinitializer vineuralnetwork)
QT5_USE_MODULES(vikimraweightinitializer Xml Multimedia)

#Install file
INSTALL(TARGETS vikimraweightinitializer DESTINATION ${VI_WEIGHT_INITIALIZER_INSTALL_PATH})
