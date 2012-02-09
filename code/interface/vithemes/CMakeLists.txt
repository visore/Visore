#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_THEME ${ROOT}/vitheme)
SET(VI_THEMES ${ROOT}/vithemes)
SET(VI_THEMES_LOCATION ${CMAKE_INSTALL_PREFIX}/interface/themes CACHE INTERNAL "Interface themes install directory")

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_THEME})
ADD_SUBDIRECTORY(${VI_THEMES})
