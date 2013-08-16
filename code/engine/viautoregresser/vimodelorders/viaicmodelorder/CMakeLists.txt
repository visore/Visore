#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AIC_MODEL_ORDER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_AIC_MODEL_ORDER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(viaicmodelorder SHARED ${VI_AIC_MODEL_ORDER_HEADERS} ${VI_AIC_MODEL_ORDER_SOURCES})
TARGET_LINK_LIBRARIES(viaicmodelorder viautoregresser)
QT5_USE_MODULES(viaicmodelorder Multimedia Xml)

#Install file
INSTALL(TARGETS viaicmodelorder DESTINATION ${VI_MODEL_ORDER_INSTALL_PATH})
