#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_warpedburg_INTERPOLATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_warpedburg_INTERPOLATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(viwarpedburginterpolator SHARED ${VI_warpedburg_INTERPOLATOR_HEADERS} ${VI_warpedburg_INTERPOLATOR_SOURCES})
TARGET_LINK_LIBRARIES(viwarpedburginterpolator viinterpolator)
QT5_USE_MODULES(viwarpedburginterpolator Xml Multimedia)

#Install file
INSTALL(TARGETS viwarpedburginterpolator DESTINATION ${VI_INTERPOLATORS_INSTALL_PATH})
