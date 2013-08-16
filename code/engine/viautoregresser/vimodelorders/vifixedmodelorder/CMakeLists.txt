#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_FIXED_MODEL_ORDER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_FIXED_MODEL_ORDER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vifixedmodelorder SHARED ${VI_FIXED_MODEL_ORDER_HEADERS} ${VI_FIXED_MODEL_ORDER_SOURCES})
TARGET_LINK_LIBRARIES(vifixedmodelorder viautoregresser)
QT5_USE_MODULES(vifixedmodelorder Multimedia Xml)

#Install file
INSTALL(TARGETS vifixedmodelorder DESTINATION ${VI_MODEL_ORDER_INSTALL_PATH})
