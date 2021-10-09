add_library(Pixie16Api SHARED pixie16.cpp $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(Pixie16Api PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET Pixie16Api USE_PLX)
install(TARGETS Pixie16Api LIBRARY DESTINATION lib)