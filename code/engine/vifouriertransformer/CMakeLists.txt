#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_FOURIERTRANSFORMER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_FOURIERTRANSFORMER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vifouriertransformer SHARED ${VI_FOURIERTRANSFORMER_HEADERS} ${VI_FOURIERTRANSFORMER_SOURCES})
TARGET_LINK_LIBRARIES(vifouriertransformer fftreal)
QT5_USE_MODULES(vifouriertransformer Core)

#Install file
INSTALL(TARGETS vifouriertransformer DESTINATION engine)
