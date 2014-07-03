#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_INTERPOLATORS_INSTALL_PATH engine/interpolators)

#Add definitions
ADD_DEFINITIONS(-DINTERPOLATORS_PATH="${VI_INTERPOLATORS_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vinearestneighbourinterpolator)
ADD_SUBDIRECTORY(${ROOT}/viprimitiveinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vipolynomialinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vifourierinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vihermiteinterpolator)
ADD_SUBDIRECTORY(${ROOT}/viarimainterpolator)
ADD_SUBDIRECTORY(${ROOT}/vigarchinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vilagrangeinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vinewtoninterpolator)
ADD_SUBDIRECTORY(${ROOT}/vineuralinterpolator)

#Initiate source files
FILE(GLOB VI_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viinterpolator SHARED ${VI_INTERPOLATOR_HEADERS} ${VI_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(viinterpolator viessentials vieigen)
QT5_USE_MODULES(viinterpolator Xml Multimedia)

#Install file
INSTALL(TARGETS viinterpolator DESTINATION engine)
