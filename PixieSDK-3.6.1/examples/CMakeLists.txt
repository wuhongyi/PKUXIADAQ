add_subdirectory(example_pixie16api)
add_subdirectory(example_pixie16app)
add_subdirectory(list_mode_file_stats)

install(DIRECTORY share DESTINATION ${CMAKE_INSTALL_PREFIX})
