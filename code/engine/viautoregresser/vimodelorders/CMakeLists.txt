SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

SET(VI_AIC_MODEL_ORDER ${ROOT}/viaicmodelorder)
SET(VI_AICC_MODEL_ORDER ${ROOT}/viaiccmodelorder)
SET(VI_FIXED_MODEL_ORDER ${ROOT}/vifixedmodelorder)
SET(VI_MAZZONI_MODEL_ORDER ${ROOT}/vimazzonimodelorder)

ADD_SUBDIRECTORY(${VI_AIC_MODEL_ORDER})
ADD_SUBDIRECTORY(${VI_AICC_MODEL_ORDER})
ADD_SUBDIRECTORY(${VI_FIXED_MODEL_ORDER})
ADD_SUBDIRECTORY(${VI_MAZZONI_MODEL_ORDER})
