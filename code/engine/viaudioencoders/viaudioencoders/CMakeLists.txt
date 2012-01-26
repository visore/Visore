#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_LAMEENCODER ${ROOT}/vilameencoder)
SET(VI_AUDIOENCODERS_LOCATION ${CMAKE_INSTALL_PREFIX}/engine/encoders CACHE INTERNAL "Audio encoders install directory")

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_LAMEENCODER})
