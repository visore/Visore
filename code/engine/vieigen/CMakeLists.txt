#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_EIGEN_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vieigen SHARED ${VI_EIGEN_HEADERS})
TARGET_LINK_LIBRARIES(vieigen ${MPFR_LIBRARIES})

#Install file
INSTALL(TARGETS vieigen DESTINATION engine)
