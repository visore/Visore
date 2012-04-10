IF(Qt5_FIND_COMPONENTS)
  
	FOREACH(component ${Qt5_FIND_COMPONENTS})
		SET(Qt5${component}_DIR ${QT5BASE}/lib/cmake/Qt5${component})
		FIND_PACKAGE(Qt5${component})
		INCLUDE_DIRECTORIES(${Qt5${component}_INCLUDE_DIRS})
	ENDFOREACH()

ENDIF(Qt5_FIND_COMPONENTS)

#For QGlobal linking errors
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
