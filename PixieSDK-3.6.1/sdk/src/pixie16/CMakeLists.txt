add_library(Pixie16ApiObjLib OBJECT pixie16.cpp)
set_property(TARGET Pixie16ApiObjLib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(Pixie16ApiObjLib PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/ ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET Pixie16ApiObjLib CONFIG_OBJ)

add_library(Pixie16Api SHARED $<TARGET_OBJECTS:Pixie16ApiObjLib> $<TARGET_OBJECTS:PixieSdkObjLib>)
target_include_directories(Pixie16Api PUBLIC ${PROJECT_SOURCE_DIR}/sdk/include/
        ${PROJECT_SOURCE_DIR}/externals/)
xia_configure_target(TARGET Pixie16Api USE_PLX)
install(TARGETS Pixie16Api LIBRARY DESTINATION lib)