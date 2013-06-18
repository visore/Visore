#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_BACKPROPAGATION_TRAINER_HEADERS ${HEADERS}/*.h)
FILE(GLOB VI_BACKPROPAGATION_TRAINER_SOURCES ${SOURCES}/*.cpp)

#Create library
ADD_LIBRARY(vibackpropagationtrainer SHARED ${VI_BACKPROPAGATION_TRAINER_HEADERS} ${VI_BACKPROPAGATION_TRAINER_SOURCES})
TARGET_LINK_LIBRARIES(vibackpropagationtrainer vineuralnetwork)
QT5_USE_MODULES(vibackpropagationtrainer Xml Multimedia)

#Install file

INSTALL(TARGETS vibackpropagationtrainer DESTINATION ${VI_TRAINER_INSTALL_PATH})
