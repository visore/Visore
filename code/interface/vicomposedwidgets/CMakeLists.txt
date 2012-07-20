#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(FORMS ${ROOT}/forms)
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Include common headers
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#Initiate source files
FILE(GLOB VI_COMPOSEDWIDGETS_FORMS ${FORMS}/*.ui ${FORMS}/*/*.ui)
FILE(GLOB VI_COMPOSEDWIDGETS_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp)
FILE(GLOB VI_COMPOSEDWIDGETS_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h)

#Wrap Qt files
QT5_WRAP_UI(VI_WRAPPER_FORMS ${VI_COMPOSEDWIDGETS_FORMS})

#Create library
ADD_LIBRARY(vicomposedwidgets SHARED ${VI_COMPOSEDWIDGETS_HEADERS} ${VI_COMPOSEDWIDGETS_SOURCES} ${VI_WRAPPER_FORMS})
TARGET_LINK_LIBRARIES(vicomposedwidgets qwt visimplewidgets vigradientcreator viaudioengine)

#Install file
INSTALL(TARGETS vicomposedwidgets DESTINATION interface)
