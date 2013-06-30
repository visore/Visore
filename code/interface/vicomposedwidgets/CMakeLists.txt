#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(FORMS ${ROOT}/forms)
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_COMPOSEDWIDGETS_FORMS ${FORMS}/*.ui ${FORMS}/*/*.ui)
FILE(GLOB VI_COMPOSEDWIDGETS_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp)
FILE(GLOB VI_COMPOSEDWIDGETS_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h)

#Create library
QT5_WRAP_UI(VI_WRAPPER_FORMS ${VI_COMPOSEDWIDGETS_FORMS})
ADD_LIBRARY(vicomposedwidgets SHARED ${VI_COMPOSEDWIDGETS_HEADERS} ${VI_COMPOSEDWIDGETS_SOURCES} ${VI_WRAPPER_FORMS})
TARGET_LINK_LIBRARIES(vicomposedwidgets qwt visimplewidgets vigradientcreator viaudioengine viinterpolationtargetprovider vinoisedetector)
QT5_USE_MODULES(vicomposedwidgets Widgets Multimedia Xml)

#Install file
INSTALL(TARGETS vicomposedwidgets DESTINATION interface)
