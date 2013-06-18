#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_ROOT_MEAN_SQUARED_ERROR_FUNCTION_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_ROOT_MEAN_SQUARED_ERROR_FUNCTION_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(virootmeansquarederrorfunction SHARED ${VI_ROOT_MEAN_SQUARED_ERROR_FUNCTION_HEADERS} ${VI_ROOT_MEAN_SQUARED_ERROR_FUNCTION_SOURCES})
TARGET_LINK_LIBRARIES(virootmeansquarederrorfunction vineuralnetwork)
QT5_USE_MODULES(virootmeansquarederrorfunction Xml Multimedia)

#Install file
INSTALL(TARGETS virootmeansquarederrorfunction DESTINATION ${VI_ERROR_FUNCTION_INSTALL_PATH})
