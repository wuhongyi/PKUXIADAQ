# SPDX-License-Identifier: Apache-2.0

# Copyright 2021 XIA LLC, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# @file FindPixieSDK.cmake
# @brief Find the PixieSDK installed on the user's system.
#
# This module defines the following variables:
# PIXIESDK_LIBRARY_DIR -> Full path to the PixieSDK lib directory.
# PIXIESDK_INCLUDE_DIR -> Full path to the PixieSDK include directory.
# PIXIESDK_LIBRARY -> Full path to the PixieSDK library.
# PIXIESDK_LIBRARIES -> Library definition with dependencies for use when linking.
# PIXIESDK_LEGACY_LIBRARY -> Full path to the legacy C API library: Pixie16App.a.
# PIXIESDK_LEGACY_LIBRARIES -> Library definition with dependencies for use when linking.
# PIXIESDK_LEGACY_INCLUDE_DIR -> Full path legacy C API headers.
# PIXIESDK_PIXIE16_LIBRARY -> Full path to the C++ compatibility wrapper: Pixie16Api.so.
# PIXIESDK_PIXIE16_LIBRARIES -> Library definition with dependencies for use when linking.
#

find_path(PIXIESDK_LIBRARY_DIR
        NAMES libPixieSDK.a PixieSDK.lib libPixie16App.a Pixie16App.lib
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS /usr/local/xia/PixieSDK "C:/Program Files (x86)/PixieSDK"
        PATH_SUFFIXES lib
        DOC "Full path to the PixieSDK lib directory."
        )

find_path(PIXIESDK_INCLUDE_DIR
        NAMES pixie legacy
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS ${PIXIESDK_LIBRARY_DIR}/../
        PATH_SUFFIXES include
        DOC "Full path to the PixieSDK include directory."
        )

find_library(PIXIESDK_LIBRARY
        NAMES PixieSDK
        HINTS $ENV{XIA_PIXIE_SDK}/lib ${PIXIESDK_LIBRARY_DIR}
        DOC "Full path to the PixieSDK library."
        )

find_library(PIXIESDK_LEGACY_LIBRARY
        NAMES Pixie16App
        HINTS $ENV{XIA_PIXIE_SDK}/lib ${PIXIESDK_LIBRARY_DIR}
        DOC "Full path to the legacy C API library: Pixie16App.a."
        )

find_path(PIXIESDK_LEGACY_INCLUDE_DIR
        NAMES pixie16app_defs.h
        HINTS $ENV{XIA_PIXIE_SDK}/include
        PATHS ${PIXIESDK_INCLUDE_DIR}
        PATH_SUFFIXES legacy
        DOC "Full path legacy C API headers."
        )

find_library(PIXIESDK_PIXIE16_LIBRARY
        NAMES Pixie16Api
        HINTS $ENV{XIA_PIXIE_SDK}/lib ${PIXIESDK_LIBRARY_DIR}
        DOC "Full path to the C++ compatibility wrapper: Pixie16Api.so."
        )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PixieSDK DEFAULT_MSG
        PIXIESDK_LIBRARY_DIR PIXIESDK_INCLUDE_DIR)

if (PixieSDK_FOUND OR PIXIESDK_FOUND)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(PIXIESDK_LIBRARIES PixieSDK dl m pthread)
        set(PIXIESDK_PIXIE16_LIBRARIES Pixie16Api PixieSDK dl m pthread)
        set(PIXIESDK_LEGACY_LIBRARIES Pixie16App dl m)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(PIXIESDK_LIBRARIES PixieSDK)
        set(PIXIESDK_PIXIE16_LIBRARIES Pixie16Api PixieSDK)
        set(PIXIESDK_LEGACY_LIBRARIES Pixie16App winmm)
    endif ()
endif ()
