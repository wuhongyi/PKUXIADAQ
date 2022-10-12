if (BUILD_LEGACY)
    install(DIRECTORY legacy/ DESTINATION include/legacy)
endif ()

if (BUILD_SDK)
    install(DIRECTORY pixie/ DESTINATION include/pixie)
endif ()

if (BUILD_PIXIE16_API)
    install(DIRECTORY pixie16/ DESTINATION include/pixie16)
endif ()