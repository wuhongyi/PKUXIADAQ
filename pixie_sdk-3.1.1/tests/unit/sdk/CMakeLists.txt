add_executable(pixie_sdk_unit_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        $<TARGET_OBJECTS:PixieSdkObjLib>
        test_pixie_buffer.cpp
        test_pixie_error.cpp
        test_pixie_log.cpp
        test_pixie_util.cpp
        )
target_include_directories(pixie_sdk_unit_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_unit_test_runner USE_PLX FORCE_DEBUG)