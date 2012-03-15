#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(VI_AUDIOENCODER ${ROOT}/viaudioencoder)
SET(VI_AUDIOENCODERS ${ROOT}/viaudioencoders)
SET(VI_AUDIOENCODERS_LOCATION engine/encoders CACHE INTERNAL "Audio encoders install directory")

#Initiate sub-directories
ADD_SUBDIRECTORY(${VI_AUDIOENCODER})
ADD_SUBDIRECTORY(${VI_AUDIOENCODERS})
