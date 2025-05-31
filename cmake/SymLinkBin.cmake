file(MAKE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")
execute_process(
	COMMAND "${CMAKE_COMMAND}" -E create_symlink
	"${CMAKE_INSTALL_PREFIX}/lib/tetraxis/tetraxis"
	"${CMAKE_INSTALL_PREFIX}/bin/tetraxis"
)
