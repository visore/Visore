#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_POLYNOMIALS_INSTALL_PATH engine/polynomials)

#Add definitions
ADD_DEFINITIONS(-DPOLYNOMIALS_PATH="${VI_POLYNOMIALS_INSTALL_PATH}")

#Add sub libraries
ADD_SUBDIRECTORY(${ROOT}/vistandardpolynomial)
ADD_SUBDIRECTORY(${ROOT}/vifourierpolynomial)
ADD_SUBDIRECTORY(${ROOT}/vinewtonpolynomial)

#Initiate source files
FILE(GLOB VI_POLYNOMIAL_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_POLYNOMIAL_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vipolynomial SHARED ${VI_POLYNOMIAL_HEADERS} ${VI_POLYNOMIAL_SOURCES})
TARGET_LINK_LIBRARIES(vipolynomial viessentials)
QT5_USE_MODULES(vipolynomial Multimedia)

#Install file
INSTALL(TARGETS vipolynomial DESTINATION engine)
