#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_NEAREST_NEIGHBOUR_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_NEAREST_NEIGHBOUR_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vinearestneighbourinterpolator SHARED ${VI_NEAREST_NEIGHBOUR_INTERPOLATOR_HEADERS} ${VI_NEAREST_NEIGHBOUR_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(vinearestneighbourinterpolator viinterpolator)
QT5_USE_MODULES(vinearestneighbourinterpolator Xml Multimedia)

#Install file
INSTALL(TARGETS vinearestneighbourinterpolator DESTINATION ${VI_INTERPOLATORS_INSTALL_PATH})
