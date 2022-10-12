if (BUILD_LEGACY)
    add_executable(example_pixie16app example_pixie16app.cpp)
    target_include_directories(example_pixie16app PUBLIC
            ${PROJECT_SOURCE_DIR}/api/legacy/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16app USE_PLX LEGACY LIBS Pixie16App)
    install(TARGETS example_pixie16app RUNTIME DESTINATION bin)
endif ()