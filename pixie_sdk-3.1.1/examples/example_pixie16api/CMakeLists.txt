if (BUILD_PIXIE16_API)
    add_executable(example_pixie16_api example_pixie16api.cpp)
    target_include_directories(example_pixie16_api PUBLIC
            ${PROJECT_SOURCE_DIR}/sdk/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16_api USE_PLX LIBS Pixie16Api)
    install(TARGETS example_pixie16_api RUNTIME DESTINATION bin)
endif ()

if (BUILD_LEGACY)
    add_executable(example_pixie16_legacy example_pixie16api.cpp)
    target_include_directories(example_pixie16_legacy PUBLIC
            ${PROJECT_SOURCE_DIR}/api/legacy/include/
            ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET example_pixie16_legacy USE_PLX LEGACY LIBS Pixie16App
            COMPILE_DEFS LEGACY_EXAMPLE)
    install(TARGETS example_pixie16_legacy RUNTIME DESTINATION bin)
endif ()

if (BUILD_SDK OR BUILD_LEGACY)
    install(FILES example_pixie16api.py DESTINATION bin)
endif ()