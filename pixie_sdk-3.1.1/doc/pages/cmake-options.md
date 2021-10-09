# CMake Options

## Primary Options

| Option | Description | Default |
|---|---|---|
| BUILD_DOCO | Tells CMake to build documentation | OFF |
| BUILD_EXAMPLES | Builds the example programs | ON |
| BUILD_LEGACY | Builds the legacy C API Pixie16App.a | OFF |
| BUILD_SDK | Builds PixieSdk.a | ON |
| BUILD_TESTS | Builds the test suites | OFF |

## Dependent Options

| Option | Description | Depends on | Default |
|---|---|---|---|
| BUILD_INTEGRATION_TESTS | Builds integration tests | BUILD_TESTS;BUILD_SDK | ON |
| BUILD_LEGACY_UNIT_TESTS | Builds legacy unit tests | BUILD_TESTS;BUILD_LEGACY | ON |
| BUILD_PIXIE16_API | Builds backward compatible Pixie16 SDK API Library | BUILD_SDK | ON |
| BUILD_SYSTEM_TESTS | Enables build of system tests | BUILD_TESTS;BUILD_SDK | ON |
| BUILD_SDK_UNIT_TESTS | Builds PixieSDK unit tests | BUILD_TESTS;BUILD_SDK | ON |
| USE_USLEEP | Adds the USE_USLEEP flag to Legacy builds | BUILD_LEGACY | OFF |