#############################################################################################
# Parameters
#############################################################################################
SET(NAME "Visore")
SET(PACKAGE "visore")
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "1")
SET(SHORT_DESCRIPTION "Vinyl Sound Refurbisher")
SET(LONG_DESCRIPTION "Visore is a software project for the sound restoration of vinyl (gramophone) records. Visore filters the noise caused by scratches, dust and degradation and amplifies the sound where necessary.")
SET(MAINTAINER "Christoph Stallmann")
SET(EMAIL "info@visore.org")
SET(HOMEPAGE "http://www.visore.org")
SET(INSTALL_LOCATION "/usr/local/visore")
SET(LICENSE "LGPLv2.1")
SET(COPYRIGHT "Copyright (c) 2011-2012 Christoph Stallmann")
SET(PACKAGE_ARCHITECTURE "i386")
IF(${ARCHITECTURE} MATCHES "64bit")
	IF(${SYSTEM} MATCHES "LINUX")
		SET(PACKAGE_ARCHITECTURE "amd64")
	ELSEIF(${SYSTEM} MATCHES "MACOSX")
		SET(PACKAGE_ARCHITECTURE "x86_64")
	ENDIF()
ENDIF()

INCLUDE(PackagingFiles)
#############################################################################################



#############################################################################################
# Basic package information
#############################################################################################
SET(CPACK_GENERATOR "DEB;RPM")
SET(CPACK_PACKAGING_INSTALL_PREFIX ${INSTALL_LOCATION})
SET(CPACK_PACKAGE_EXECUTABLES launcher;visore)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${README_FILE})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${SHORT_DESCRIPTION})
SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${PACKAGE_ARCHITECTURE}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${NAME} ${VERSION_MAJOR}.${VERSION_MINOR}")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(CPACK_PACKAGE_NAME ${NAME})
SET(CPACK_PACKAGE_VENDOR ${NAME})
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${PACKAGE_ARCHITECTURE}")
SET(CPACK_RESOURCE_FILE_LICENSE ${COPYRIGHT_FILE})
SET(CPACK_RESOURCE_FILE_README ${README_FILE})
#############################################################################################



#############################################################################################
# Debian package information (.deb)
#############################################################################################
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "${MAINTAINER} <${EMAIL}>")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${PACKAGE_ARCHITECTURE})
SET(CPACK_DEBIAN_PACKAGE_SECTION sound)
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION_SUMMARY ${SHORT_DESCRIPTION})
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${SHORT_DESCRIPTION}
	${LONG_DESCRIPTION}
Homepage: ${HOMEPAGE}")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-core (>= 4.7), libqt4-gui (>= 4.7)")
SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS "lame (>= 3.98), flac (>= 1.2)")
#############################################################################################



#############################################################################################
# Redhat package information (.rpm)
#############################################################################################
SET(CPACK_RPM_PACKAGE_PROVIDES "libbass.so")
SET(CPACK_RPM_PACKAGE_SUMMARY ${SHORT_DESCRIPTION})
SET(CPACK_RPM_PACKAGE_ARCHITECTURE ${PACKAGE_ARCHITECTURE})
SET(CPACK_RPM_PACKAGE_LICENSE ${LICENSE})
SET(CPACK_RPM_PACKAGE_GROUP "Applications/Multimedia")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "${SHORT_DESCRIPTION}
${LONG_DESCRIPTION}")
#set(CPACK_RPM_PACKAGE_REQUIRES "python >= 2.5.0, cmake >= 2.8")
#############################################################################################



INCLUDE(CPack)
