IF(UNIX)
	CONFIGURE_FILE(
		TBTKCreateApplication
		TBTKCreateApplication
	)
	INSTALL(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/TBTKCreateApplication
		DESTINATION TBTK
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
	INSTALL(
		CODE "EXECUTE_PROCESS(
			COMMAND ${CMAKE_COMMAND}
			-E create_symlink
			${CMAKE_INSTALL_PREFIX}/TBTK/TBTKCreateApplication
			${CMAKE_INSTALL_PREFIX}/bin/TBTKCreateApplication
		)"
	)
ENDIF(UNIX)
