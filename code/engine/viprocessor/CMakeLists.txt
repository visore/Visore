
#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Libraries
SET(VI_NEURAL_NETWORK ${ROOT}/vineuralnetwork)
ADD_SUBDIRECTORY(${VI_NEURAL_NETWORK})

#Initiate source files
FILE(GLOB VI_PROCESSOR_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp ${SOURCES}/*/*/*.cpp)
FILE(GLOB VI_PROCESSOR_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h ${HEADERS}/*/*/*.h)

#Create library
ADD_LIBRARY(viprocessor SHARED ${VI_PROCESSOR_HEADERS} ${VI_PROCESSOR_SOURCES})
TARGET_LINK_LIBRARIES(viprocessor viaudioconnection vifouriertransformer visongidentifier viproject vineuralnetwork viinterpolator)

#Install file
INSTALL(TARGETS viprocessor DESTINATION engine)
