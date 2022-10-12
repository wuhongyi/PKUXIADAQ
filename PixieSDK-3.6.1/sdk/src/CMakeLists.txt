if (BUILD_LEGACY)
    add_subdirectory(legacy)
endif ()

if (BUILD_SDK)
    add_subdirectory(pixie)
endif ()

if (BUILD_PIXIE16_API)
    add_subdirectory(pixie16)
endif ()