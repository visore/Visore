#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_AUDIOCODER_SOURCES ${SOURCES}/*.cpp ${SOURCES}/*/*.cpp)
FILE(GLOB VI_AUDIOCODER_HEADERS ${HEADERS}/*.h ${HEADERS}/*/*.h)

#Create library
ADD_LIBRARY(viaudiocoder SHARED ${VI_AUDIOCODER_HEADERS} ${VI_AUDIOCODER_SOURCES})
TARGET_LINK_LIBRARIES(viaudiocoder viessentials)

#Install file
INSTALL(TARGETS viaudiocoder DESTINATION engine)
