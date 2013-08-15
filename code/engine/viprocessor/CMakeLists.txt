
#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Add definitions
SET(VI_PROCESSOR_INSTALL_PATH engine/processors)

#Libraries
ADD_SUBDIRECTORY(${ROOT}/vineuralnetwork)
ADD_SUBDIRECTORY(${ROOT}/vicorrelator)
ADD_SUBDIRECTORY(${ROOT}/vispectrumanalyzer)
ADD_SUBDIRECTORY(${ROOT}/viinterpolationcorrector)

#Initiate source files
FILE(GLOB VI_PROCESSOR_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp ${SOURCES}/*/*/*.cpp)
FILE(GLOB VI_PROCESSOR_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h ${HEADERS}/*/*/*.h)

#Create library
ADD_LIBRARY(viprocessor SHARED ${VI_PROCESSOR_HEADERS} ${VI_PROCESSOR_SOURCES})
TARGET_LINK_LIBRARIES(viprocessor viaudioconnection vifouriertransformer visongidentifier viproject vineuralnetwork viinterpolator)
QT5_USE_MODULES(viprocessor Multimedia Xml)

#Install file
INSTALL(TARGETS viprocessor DESTINATION ${VI_PROCESSOR_INSTALL_PATH})
