#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_PROJECT_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp)
FILE(GLOB VI_PROJECT_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h)

#Create library
ADD_LIBRARY(viproject SHARED ${VI_PROJECT_HEADERS} ${VI_PROJECT_SOURCES})
TARGET_LINK_LIBRARIES(viproject viaudiocoder viaudioobject)
QT5_USE_MODULES(viproject Multimedia Xml)

#Install file
INSTALL(TARGETS viproject DESTINATION engine)
