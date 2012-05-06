#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)
SET(VI_BASSCONNECTION_HEADERS ${ENGINE}/viaudioconnections/viaudioconnections/vibassconnection/headers)
SET(VI_QTCONNECTION_HEADERS ${ENGINE}/viaudioconnections/viaudioconnections/viqtconnection/headers)
SET(BASS_HEADERS ${EXTERNAL}/bass/headers)

#Include common headers
INCLUDE_DIRECTORIES(${HEADERS} ${VI_BASSCONNECTION_HEADERS} ${VI_QTCONNECTION_HEADERS} ${BASS_HEADERS})

#Initiate source files
FILE(GLOB VI_AUDIOENGINE_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_AUDIOENGINE_HEADERS_ALL ${HEADERS}/*.h ${ENGINE}/viaudioconnections/viaudioconnection/headers/*.h)
SET(VI_AUDIOENGINE_HEADERS)
SET(VI_AUDIOENGINE_MOC_HEADERS)
FOREACH(HEADER ${VI_AUDIOENGINE_HEADERS_ALL})
    FILE(STRINGS "${HEADER}" lines REGEX "Q_OBJECT")
    IF(lines)
        LIST(APPEND VI_AUDIOENGINE_MOC_HEADERS "${HEADER}")
	ELSE()
		LIST(APPEND VI_AUDIOENGINE_HEADERS "${HEADER}")
    ENDIF()
ENDFOREACH()

#Wrap Qt files
QT5_WRAP_CPP(VI_WRAPPER_HEADERS ${VI_AUDIOENGINE_MOC_HEADERS})

#Create library
ADD_LIBRARY(viaudioengine SHARED ${VI_AUDIOENGINE_HEADERS} ${VI_AUDIOENGINE_SOURCES} ${VI_WRAPPER_HEADERS})
TARGET_LINK_LIBRARIES(viaudioengine ${QT_LIBRARIES} viessentials viaudioconnection viqtconnection viwaveformer visongdetector vimatcher)

#Install file
INSTALL(TARGETS viaudioengine DESTINATION engine)
