add_executable(pixie_sdk_integration_test_runner $<TARGET_OBJECTS:BaseTestRunnerObjLib>
        src/test_parameter_read_write.cpp
        $<TARGET_OBJECTS:PixieSdkObjLib>
        )
target_include_directories(pixie_sdk_integration_test_runner PUBLIC
        ${PROJECT_SOURCE_DIR}/sdk/include
        ${PROJECT_SOURCE_DIR}/externals/
        ${PLX_INCLUDE_DIR})
xia_configure_target(TARGET pixie_sdk_integration_test_runner USE_PLX FORCE_DEBUG)
