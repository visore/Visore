#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_MAIN_SOURCES ${SOURCES}/*.cpp)

#Create executable
ADD_EXECUTABLE(visore ${VI_MAIN_SOURCES})

#Link Visore to Qt
TARGET_LINK_LIBRARIES(visore ${QT_LIBRARIES} viaudioengine)

#Install file
INSTALL(TARGETS visore RUNTIME DESTINATION .)
