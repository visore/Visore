#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_WIDGET ${ROOT}/viwidget)
SET(VI_WIDGETS ${ROOT}/viwidgets)

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_WIDGET})
ADD_SUBDIRECTORY(${VI_WIDGETS})
