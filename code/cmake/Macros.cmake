# FILE_DIRECTORIES without paths to exclude
IF(0)

MACRO(FILE_DIRECTORIES return_list)
	FILE(GLOB_RECURSE header_list *.h)
	FILE(GLOB_RECURSE source_list *.cpp)
	SET(dir_list "")
	FOREACH(file_path ${header_list})
		GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		SET(dir_list ${dir_list} ${dir_path})
	ENDFOREACH()
	FOREACH(file_path ${source_list})
		GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		SET(dir_list ${dir_list} ${dir_path})
	ENDFOREACH()
	LIST(REMOVE_DUPLICATES dir_list)
	SET(${return_list} ${dir_list})
ENDMACRO()

ENDIF()

# FILE_DIRECTORIES with paths to exclude
#IF(0)

MACRO(FILE_DIRECTORIES return_list exclude_list)
	FILE(GLOB_RECURSE header_list *.h)
	FILE(GLOB_RECURSE source_list *.cpp)
	SET(dir_list "")
	FOREACH(file_path ${header_list})
		GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		SET(include TRUE)
		FOREACH(exclude_file_path ${exclude_list})
			IF(${dir_path} MATCHES "(.*${exclude_file_path}.*)")
				SET(include FALSE)
			ENDIF()
		ENDFOREACH()
		IF(${include})		
			SET(dir_list ${dir_list} ${dir_path})
		ENDIF()
	ENDFOREACH()
	FOREACH(file_path ${source_list})
		GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
		SET(include TRUE)
		FOREACH(exclude_file_path ${exclude_list})
			IF(${dir_path} MATCHES "(.*${exclude_file_path}.*)")
				SET(include FALSE)
			ENDIF()
		ENDFOREACH()
		IF(${include})		
			SET(dir_list ${dir_list} ${dir_path})
		ENDIF()
	ENDFOREACH()
	LIST(REMOVE_DUPLICATES dir_list)
	SET(${return_list} ${dir_list})
ENDMACRO()

#ENDIF()
