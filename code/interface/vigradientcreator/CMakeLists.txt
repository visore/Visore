#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_GRADIENTCREATOR_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_GRADIENTCREATOR_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vigradientcreator SHARED ${VI_GRADIENTCREATOR_HEADERS} ${VI_GRADIENTCREATOR_SOURCES})
TARGET_LINK_LIBRARIES(vigradientcreator vitheme)
QT5_USE_MODULES(vigradientcreator Gui)

#Install file
INSTALL(TARGETS vigradientcreator DESTINATION interface)
