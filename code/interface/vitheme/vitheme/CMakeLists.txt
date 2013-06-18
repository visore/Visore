#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_THEME_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_THEME_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vitheme SHARED ${VI_THEME_HEADERS} ${VI_THEME_SOURCES})
TARGET_LINK_LIBRARIES(vitheme viessentials)
QT5_USE_MODULES(vitheme Gui Multimedia)

#Install file
INSTALL(TARGETS vitheme DESTINATION interface)
