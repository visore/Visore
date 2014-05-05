#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_ARMA_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_ARMA_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viarmainterpolator SHARED ${VI_ARMA_INTERPOLATOR_HEADERS} ${VI_ARMA_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(viarmainterpolator viinterpolator vigretl)
QT5_USE_MODULES(viarmainterpolator Xml Multimedia)

#Install file
INSTALL(TARGETS viarmainterpolator DESTINATION ${VI_INTERPOLATORS_INSTALL_PATH})
