#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MAZZONI_MODEL_ORDER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_MAZZONI_MODEL_ORDER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vimazzonimodelorder SHARED ${VI_MAZZONI_MODEL_ORDER_HEADERS} ${VI_MAZZONI_MODEL_ORDER_SOURCES})
TARGET_LINK_LIBRARIES(vimazzonimodelorder viaicmodelorder)
QT5_USE_MODULES(vimazzonimodelorder Multimedia Xml)

#Install file
INSTALL(TARGETS vimazzonimodelorder DESTINATION ${VI_MODEL_ORDER_INSTALL_PATH})
