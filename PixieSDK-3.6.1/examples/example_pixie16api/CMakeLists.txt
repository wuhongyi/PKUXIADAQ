if (BUILD_PIXIE16_API)
    add_executable(example_pixie16api example_pixie16api.cpp)
    target_include_directories(example_pixie16api PUBLIC
            ${PROJECT_SOURCE_DIR}/sdk/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16api USE_PLX LIBS Pixie16Api)
    install(TARGETS example_pixie16api RUNTIME DESTINATION bin)
    install(FILES example_pixie16api.py DESTINATION bin)
endif ()