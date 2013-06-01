#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_INTERPOLATORS_INSTALL_PATH engine/interpolators)

#Add definitions
ADD_DEFINITIONS(-DINTERPOLATORS_PATH="${VI_INTERPOLATORS_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vilinearinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vicosineinterpolator)
ADD_SUBDIRECTORY(${ROOT}/vicubicinterpolator)

#Initiate source files
FILE(GLOB VI_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viinterpolator SHARED ${VI_INTERPOLATOR_HEADERS} ${VI_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(viinterpolator viessentials)

#Install file
INSTALL(TARGETS viinterpolator DESTINATION engine)