#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_SUM_OF_SQUARED_ERROR_FUNCTION_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_SUM_OF_SQUARED_ERROR_FUNCTION_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(visumofsquarederrorfunction SHARED ${VI_SUM_OF_SQUARED_ERROR_FUNCTION_HEADERS} ${VI_SUM_OF_SQUARED_ERROR_FUNCTION_SOURCES})
TARGET_LINK_LIBRARIES(visumofsquarederrorfunction vineuralnetwork)
QT5_USE_MODULES(visumofsquarederrorfunction Xml Multimedia)

#Install file
INSTALL(TARGETS visumofsquarederrorfunction DESTINATION ${VI_ERROR_FUNCTION_INSTALL_PATH})
