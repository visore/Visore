#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_INTERPOLATION_TARGET_PROVIDER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_INTERPOLATION_TARGET_PROVIDER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(viinterpolationtargetprovider SHARED ${VI_INTERPOLATION_TARGET_PROVIDER_HEADERS} ${VI_INTERPOLATION_TARGET_PROVIDER_SOURCES})
TARGET_LINK_LIBRARIES(viinterpolationtargetprovider vineuralnetwork)
QT5_USE_MODULES(viinterpolationtargetprovider Xml Multimedia)

#Install file
INSTALL(TARGETS viinterpolationtargetprovider DESTINATION ${VI_TARGET_PROVIDER_INSTALL_PATH})
