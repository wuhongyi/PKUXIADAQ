add_executable(legacy_unit_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        test_pixie16app.cpp
        test_utilities.cpp
        test_tools.cpp
        $<TARGET_OBJECTS:LegacyObjLib>)
target_include_directories(legacy_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include/legacy
        ${PROJECT_SOURCE_DIR}/externals/
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET legacy_unit_test_runner USE_PLX PLX_STATIC FORCE_DEBUG
        LINUX_LIBS -Wl,--no-as-needed LEGACY)