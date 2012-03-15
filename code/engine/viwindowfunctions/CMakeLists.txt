#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_WINDOWFUNCTION ${ROOT}/viwindowfunction)
SET(VI_WINDOWFUNCTIONS ${ROOT}/viwindowfunctions)
SET(VI_WINDOWFUNCTIONS_LOCATION engine/windowfunctions CACHE INTERNAL "Window functions install directory")

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_WINDOWFUNCTION})
ADD_SUBDIRECTORY(${VI_WINDOWFUNCTIONS})
