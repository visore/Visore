#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_SONGIDENTIFIER_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp)
FILE(GLOB VI_SONGIDENTIFIER_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h)

#Create library
ADD_LIBRARY(visongidentifier SHARED ${VI_SONGIDENTIFIER_HEADERS} ${VI_SONGIDENTIFIER_SOURCES})
TARGET_LINK_LIBRARIES(visongidentifier viaudiobuffer ${ENMFP_LIBRARY} acoustid)
QT5_USE_MODULES(visongidentifier Network Gui Multimedia Xml)

#Install file
INSTALL(TARGETS visongidentifier DESTINATION engine)
