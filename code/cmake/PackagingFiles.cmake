#############################################################################################
# Debian desktop/menu shortcut file
#############################################################################################
IF(${SYSTEM} STREQUAL "LINUX")
	SET(DESKTOP_FILE ${CMAKE_BINARY_DIR}/cpack/Visore.desktop)
	FILE(WRITE ${DESKTOP_FILE}
	"[Desktop Entry]
Name=${NAME}
Version=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
Exec=${INSTALL_LOCATION}/launcher
Comment=${SHORT_DESCRIPTION}
Icon=${INSTALL_LOCATION}/logo.png
Type=Application
Terminal=false
StartupNotify=true
Encoding=UTF-8
Categories=Audio;AudioVideo;"
	)
	INSTALL(FILES ${DESKTOP_FILE} DESTINATION ../../share/applications)
ELSEIF(${SYSTEM} STREQUAL "MACOSX")
	SET(PROPERTY_FILE ${CMAKE_BINARY_DIR}/cpack/Info.plist)
	FILE(WRITE ${PROPERTY_FILE}
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleExecutable</key>
	<string>launcher</string>
	<key>CFBundleIconFile</key>
	<string>logo.icns</string>
	<key>CFBundleIdentifier</key>
	<string>org.visore.visore</string>
	<key>CFBundleName</key>
	<string>${NAME}</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}</string>
	<key>LSApplicationCategoryType</key>
	<string>public.app-category.music</string>
	<key>LSMinimumSystemVersion</key>
	<string>10.5</string>
	<key>CFBundleDisplayName</key>
	<string>Visore</string>
	<key>NSSupportsSuddenTermination</key>
	<true/>
</dict>
</plist>"
	)
	INSTALL(FILES ${PROPERTY_FILE} DESTINATION ..)

	SET(INTRODUCTION_FILE ${CMAKE_BINARY_DIR}/cpack/indroduction.txt)
	FILE(WRITE ${INTRODUCTION_FILE}
	"Welcome to the installer for ${NAME} ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.

Visore (Vinyl Sound Refurbisher) is an application for the sound restoration of vinyl (gramophone) records. Visore filters the noise caused by scratches, dust and degradation and amplifies the sound where necessary."
	)
ENDIF()
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

License: ${LICENSE}

${COPYRIGHT}
"
)
INSTALL(FILES ${COPYRIGHT_FILE} DESTINATION .)
#############################################################################################
