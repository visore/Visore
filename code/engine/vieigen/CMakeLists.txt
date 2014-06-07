#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_EIGEN_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_EIGEN_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vieigen SHARED ${VI_EIGEN_HEADERS} ${VI_EIGEN_SOURCES})
TARGET_LINK_LIBRARIES(vieigen viessentials ${MPFR_LIBRARIES})
QT5_USE_MODULES(vieigen Core Multimedia)

#Install file
INSTALL(TARGETS vieigen DESTINATION engine)
