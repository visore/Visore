#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_FUNCTIONS_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_FUNCTIONS_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vifunctions SHARED ${VI_FUNCTIONS_HEADERS} ${VI_FUNCTIONS_SOURCES})
TARGET_LINK_LIBRARIES(vifunctions viessentials)
QT5_USE_MODULES(vifunctions Core)

#Install file
INSTALL(TARGETS vifunctions DESTINATION engine)
