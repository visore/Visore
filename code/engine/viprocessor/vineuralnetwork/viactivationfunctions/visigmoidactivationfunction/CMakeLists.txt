#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_SIGMOID_ACTIVATION_FUNCTION_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_SIGMOID_ACTIVATION_FUNCTION_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(visigmoidactivationfunction SHARED ${VI_SIGMOID_ACTIVATION_FUNCTION_HEADERS} ${VI_SIGMOID_ACTIVATION_FUNCTION_SOURCES})
TARGET_LINK_LIBRARIES(visigmoidactivationfunction vineuralnetwork)
QT5_USE_MODULES(visigmoidactivationfunction Xml Multimedia)

#Install file
INSTALL(TARGETS visigmoidactivationfunction DESTINATION ${VI_ACTIVATION_FUNCTION_INSTALL_PATH})
