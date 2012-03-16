#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(LOGOS ${ROOT}/logos)

#Install file
INSTALL(FILES ${LOGOS}/logo256.png DESTINATION .)
