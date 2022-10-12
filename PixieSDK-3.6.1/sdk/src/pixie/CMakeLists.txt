add_subdirectory(data)
add_subdirectory(pixie16)

set(SDK_COMMON_SOURCES
        buffer.cpp
        config.cpp
        eeprom.cpp
        error.cpp
        fw.cpp
        log.cpp
        param.cpp
        stats.cpp
        util.cpp
        )

add_library(PixieSdkObjLib OBJECT ${SDK_PIXIE16_SOURCES} ${SDK_COMMON_SOURCES})
set_property(TARGET PixieSdkObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(PixieSdkObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieSdkObjLib USE_PLX CONFIG_OBJ)

add_library(PixieSDK STATIC $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(PixieSDK PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET PixieSDK USE_PLX)

install(TARGETS PixieSDK ARCHIVE DESTINATION lib)