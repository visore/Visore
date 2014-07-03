#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_NEURAL_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_NEURAL_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vineuralinterpolator SHARED ${VI_NEURAL_INTERPOLATOR_HEADERS} ${VI_NEURAL_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(vineuralinterpolator viinterpolator vifann)
QT5_USE_MODULES(vineuralinterpolator Xml Multimedia)

#Install file
INSTALL(TARGETS vineuralinterpolator DESTINATION ${VI_INTERPOLATORS_INSTALL_PATH})
