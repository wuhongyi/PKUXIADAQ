if (BUILD_LEGACY_UNIT_TESTS)
    add_subdirectory(legacy)
endif ()

if (BUILD_SDK_UNIT_TESTS)
    add_subdirectory(sdk)
endif ()