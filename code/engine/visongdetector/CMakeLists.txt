#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_SONGDETECTOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_SONGDETECTOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(visongdetector SHARED ${VI_SONGDETECTOR_HEADERS} ${VI_SONGDETECTOR_SOURCES})
TARGET_LINK_LIBRARIES(visongdetector viaudiocoder ${ECHONEST_LIBRARY})
QT5_USE_MODULES(visongdetector Network)

#Install file
INSTALL(TARGETS visongdetector DESTINATION engine)
