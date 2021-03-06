#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Setup install paths
SET(VI_NEURAL_NETWORK_INSTALL_PATH engine/processors/neuralnetwork)
SET(VI_ACTIVATION_FUNCTION_INSTALL_PATH ${VI_NEURAL_NETWORK_INSTALL_PATH}/activationfunctions)
SET(VI_WEIGHT_INITIALIZER_INSTALL_PATH ${VI_NEURAL_NETWORK_INSTALL_PATH}/weightinitializers)
SET(VI_ERROR_FUNCTION_INSTALL_PATH ${VI_NEURAL_NETWORK_INSTALL_PATH}/errorfunctions)
SET(VI_TRAINER_INSTALL_PATH ${VI_NEURAL_NETWORK_INSTALL_PATH}/trainers)
SET(VI_TARGET_PROVIDER_INSTALL_PATH ${VI_NEURAL_NETWORK_INSTALL_PATH}/targetproviders)

#Add definitions
ADD_DEFINITIONS(-DNEURAL_NETWORK_PATH="${VI_NEURAL_NETWORK_INSTALL_PATH}")
ADD_DEFINITIONS(-DACTIVATION_FUNCTION_PATH="${VI_ACTIVATION_FUNCTION_INSTALL_PATH}")
ADD_DEFINITIONS(-DWEIGHT_INITIALIZER_PATH="${VI_WEIGHT_INITIALIZER_INSTALL_PATH}")
ADD_DEFINITIONS(-DERROR_FUNCTION_PATH="${VI_ERROR_FUNCTION_INSTALL_PATH}")
ADD_DEFINITIONS(-DTRAINER_PATH="${VI_TRAINER_INSTALL_PATH}")
ADD_DEFINITIONS(-DTARGET_PROVIDER_PATH="${VI_TARGET_PROVIDER_INSTALL_PATH}")

#Add sub libraries
SET(VI_ACTIVATION_FUNCTIONS ${ROOT}/viactivationfunctions)
SET(VI_WEIGHT_INITIALIZERS ${ROOT}/viweightinitializers)
SET(VI_ERROR_FUNCTIONS ${ROOT}/vierrorfunctions)
SET(VI_TRAINERS ${ROOT}/vitrainers)
SET(VI_TARGET_PROVIDERS ${ROOT}/vitargetproviders)
ADD_SUBDIRECTORY(${VI_ACTIVATION_FUNCTIONS})
ADD_SUBDIRECTORY(${VI_WEIGHT_INITIALIZERS})
ADD_SUBDIRECTORY(${VI_ERROR_FUNCTIONS})
ADD_SUBDIRECTORY(${VI_TRAINERS})
ADD_SUBDIRECTORY(${VI_TARGET_PROVIDERS})

#Initiate source files
FILE(GLOB VI_NEURAL_NETWORK_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp ${SOURCES}/*/*/*.cpp)
FILE(GLOB VI_NEURAL_NETWORK_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h ${HEADERS}/*/*/*.h)

#Create library
ADD_LIBRARY(vineuralnetwork SHARED ${VI_NEURAL_NETWORK_HEADERS} ${VI_NEURAL_NETWORK_SOURCES})
TARGET_LINK_LIBRARIES(vineuralnetwork viessentials vifunctions)
QT5_USE_MODULES(vineuralnetwork Multimedia Xml)

#Install file
INSTALL(TARGETS vineuralnetwork DESTINATION ${VI_NEURAL_NETWORK_INSTALL_PATH})
