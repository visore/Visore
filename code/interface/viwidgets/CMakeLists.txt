#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_WIDGET ${ROOT}/viwidget)
SET(VI_WIDGETS ${ROOT}/viwidgets)
SET(VI_WIDGETSS_LOCATION ${CMAKE_INSTALL_PREFIX}/interface/widgets CACHE INTERNAL "Widgets install directory")

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_WIDGET})
ADD_SUBDIRECTORY(${VI_WIDGETS})
