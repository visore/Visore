#############################################################################################
# Debian desktop/menu shortcut file
#############################################################################################
SET(DEBIAN_DESKTOP_FILE ${CMAKE_BINARY_DIR}/cpack/Visore.desktop)
FILE(WRITE ${DEBIAN_DESKTOP_FILE}
"[Desktop Entry]
Name=${NAME}
Version=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
Exec=${INSTALL_LOCATION}/launcher
Comment=${SHORT_DESCRIPTION}
Icon=${INSTALL_LOCATION}/logo256.png
Type=Application
Terminal=false
StartupNotify=true
Encoding=UTF-8
Categories=Audio;AudioVideo;"
)
INSTALL(FILES ${DEBIAN_DESKTOP_FILE} DESTINATION ../../share/applications)
#############################################################################################



#############################################################################################
# Readme File
#############################################################################################
SET(README_FILE ${CMAKE_BINARY_DIR}/cpack/readme.txt)
FILE(WRITE ${README_FILE}
"----------------------------------------------------------------------------
${NAME} ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
${SHORT_DESCRIPTION}
----------------------------------------------------------------------------
Maintainer: ${MAINTAINER}
Email: ${EMAIL}
Website: ${HOMEPAGE}
----------------------------------------------------------------------------

${LONG_DESCRIPTION}
"
)
INSTALL(FILES ${README_FILE} DESTINATION .)
#############################################################################################



#############################################################################################
# Copyright File
#############################################################################################
SET(COPYRIGHT_FILE ${CMAKE_BINARY_DIR}/cpack/copyright.txt)
FILE(WRITE ${COPYRIGHT_FILE}
"----------------------------------------------------------------------------
${NAME} ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
${SHORT_DESCRIPTION}
----------------------------------------------------------------------------
Maintainer: ${MAINTAINER}
Email: ${EMAIL}
Website: ${HOMEPAGE}
----------------------------------------------------------------------------

${LICENSE}
"
)
INSTALL(FILES ${COPYRIGHT_FILE} DESTINATION .)
#############################################################################################
