#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MEAN_ABSOLUTE_ERROR_FUNCTION_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_MEAN_ABSOLUTE_ERROR_FUNCTION_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(viabsolutesquarederrorfunction SHARED ${VI_MEAN_ABSOLUTE_ERROR_FUNCTION_HEADERS} ${VI_MEAN_ABSOLUTE_ERROR_FUNCTION_SOURCES})
TARGET_LINK_LIBRARIES(viabsolutesquarederrorfunction vineuralnetwork)
QT5_USE_MODULES(viabsolutesquarederrorfunction Xml Multimedia)

#Install file
INSTALL(TARGETS viabsolutesquarederrorfunction DESTINATION ${VI_ERROR_FUNCTION_INSTALL_PATH})
