if (BUILD_LEGACY)
    add_executable(p16_371_legacy src/p16_371.cpp)
    target_include_directories(p16_371_legacy PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET p16_371_legacy USE_PLX LIBS Pixie16App PixieSDK COMPILE_DEFS LEGACY_SWITCH)
endif ()

if (BUILD_SDK)
    add_executable(pixie16_omnitool src/pixie16_omnitool.cpp $<TARGET_OBJECTS:PixieSdkObjLib>)
    target_include_directories(pixie16_omnitool PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
    xia_configure_target(TARGET pixie16_omnitool USE_PLX)

    if (BUILD_PIXIE16_API)
        add_executable(p16_246 src/p16_246.cpp)
        target_include_directories(p16_246 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_246 USE_PLX LIBS PixieSDK)

        add_executable(p16_371 src/p16_371.cpp)
        target_include_directories(p16_371 PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include ${PROJECT_SOURCE_DIR}/externals/)
        xia_configure_target(TARGET p16_371 USE_PLX LIBS Pixie16Api)
    endif ()
endif ()

